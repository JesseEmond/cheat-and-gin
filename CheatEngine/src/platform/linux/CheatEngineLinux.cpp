#include "../../CheatEngine.h"

#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

#include <iostream>
#include <string>
#include <tuple>
#include <fstream>
#include <sstream>
#include <regex>
#include <cassert>

using namespace std;

// extracts the pid and name of the given process directory entry
// within /proc. If we fail to extract the information, throw
// an invalid_argument exception.
pair<pid_t, string> get_process_pid_name(const dirent* procEntry);
// parses a line from a /proc/[pid]/maps file to extract the start/end
// addresses and return whether the parsing was a success AND that the
// memory page is one that we care about (i.e. we can read/write it and
// it is private memory).
bool parse_memory_map_line(const string& line, pair<long,long>& addresses);
// reads a process chunk of memory starting at the given address offset with a
// given size.
bytes_t read_memory_chunk(phandle_t process, pid_t pid, long start, size_t size);
// extracts the matching memory offsets with a given value within a block of memory.
offsets_t extract_matches(const bytes_t& memory, const bytes_t& searched);
// removes the memory offsets that no longer fit a given value.
void remove_matches_with_different_value(phandle_t process, pid_t pid,
  MemoryBlock& block, const bytes_t& value);

//TODO: custom object for that lock for idiomatic RAII
// locks the execution of the target process.
void lock_process(pid_t pid);
// unlocks the execution of the target process.
void unlock_process(pid_t pid);



void CheatEngine::addAddressesWithValue(const value_t& value, value_size_t size) {
  lock_process(m_processId);

  stringstream memoryMapFilename;
  memoryMapFilename << "/proc/" << m_processId << "/maps";
  ifstream memMap(memoryMapFilename.str());

  if (!memMap) {
    cerr << "Failed to open memory map file for pid " << m_processId << endl;
    return;
  }

  string line;
  while (getline(memMap, line)) {
    pair<long,long> page;
    if (parse_memory_map_line(line, page)) {
      assert(page.second > page.first);
      MemoryBlock block;
      block.baseAddress = address_t(0) + page.first;
      block.size = page.second - page.first;

      auto memory = read_memory_chunk(m_process, m_processId, page.first, block.size);
      block.matches = extract_matches(memory, value);

      if (!block.matches.empty()) {
        m_blocks.push_back(block);
      }
    }
  }

  unlock_process(m_processId);
}

void CheatEngine::keepAddressesWithValue(const value_t& value, value_size_t size) {
  lock_process(m_processId);

  for_each(m_blocks.begin(), m_blocks.end(), [&](MemoryBlock& block) {
    remove_matches_with_different_value(m_process, m_processId, block, value);
  });

  // clean empty blocks
  m_blocks.erase(remove_if(m_blocks.begin(), m_blocks.end(), [](const MemoryBlock& block) {
      return block.matches.empty();
    }),
    m_blocks.cend());

  unlock_process(m_processId);
}

void CheatEngine::modifyMatchingAddresses(const value_t& value, value_size_t size) const {
}

vector<pid_t> CheatEngine::getProcessesWithName(const std::string& name) {
  vector<pid_t> processes;
  auto dir = opendir("/proc");
  if (dir != nullptr) {
    dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
      try {
        pid_t pid;
        string pname;

        tie(pid, pname) = get_process_pid_name(entry);
        if (name == pname) {
          processes.push_back(pid);
        }
      } catch (invalid_argument) {} // could not read? ignore error
    }
    closedir(dir);
  } else {
    cerr << "Failed to open /proc to iterate over running processes." << endl;
  }
  return processes;
}

phandle_t CheatEngine::openProcess(pid_t id) const {
  stringstream memFilename;
  memFilename << "/proc/" << id << "/mem";

  phandle_t memoryFile = open(memFilename.str().c_str(), O_RDONLY);
  if (memoryFile < 0) {
    cerr << "Failed to open process memory file. Do you have the required rights?" << endl;
    exit(1);
  }

  return memoryFile;
}

void CheatEngine::closeProcess(pid_t id, phandle_t handle) const {
  if (close(handle) < 0) {
    cerr << "Failed to close process memory file." << endl;
    exit(1);
  }
}

pair<pid_t, string> get_process_pid_name(const dirent* procEntry) {
  string pidStr(procEntry->d_name);
  pid_t pid = stol(pidStr); // let it throw invalid_argument if fails

  stringstream procNameFilename;
  procNameFilename << "/proc/" << pid << "/comm";
  ifstream procNameFile(procNameFilename.str());

  if (!procNameFile) {
    cerr << "Failed to open comm file for pid " << pid << endl;
    throw invalid_argument("Faile to open comm file for process");
  }

  string name;
  procNameFile >> name;
  return make_pair(pid, name);
}

bool parse_memory_map_line(const string& line, pair<long,long>& addresses) {
  static const regex pattern("([0-9A-Fa-f]+)-([0-9A-Fa-f]+) ([-r])([-w])[-x]([sp]).*");

  smatch matches;
  regex_match(line, matches, pattern);

  if (matches.size() != 6) {
    cerr << "Could not properly parse line in maps file. Line: " << line << endl;
    return false;
  }

  auto start = matches[1];
  auto end = matches[2];
  auto read = matches[3];
  auto write = matches[4];
  auto privacy = matches[5];

  if (read    == "r" &&
      write   == "w" &&
      privacy == "p") {
    try {
      auto startAddress = stol(start, nullptr, 16);
      auto endAddress   = stol(end,   nullptr, 16);

      addresses.first  = startAddress;
      addresses.second = endAddress;

      return true;
    } catch (invalid_argument) {
      cerr << "Failed to parse start or end date of mmap: " << start
        << " " << end << endl;
      return false;
    }
  }

  return false;
}

bytes_t read_memory_chunk(phandle_t process, pid_t pid, long start, size_t size) {
  bytes_t memory(size);
  auto offset = lseek(process, start, SEEK_SET);

  if (offset != start) {
    cerr << "Failed to seek in memory file for process " << pid << endl;
    return bytes_t();
  }

  if (read(process, memory.data(), size) < 0) {
    cerr << "Error while reading memory from memory file for process "
         << pid << endl;
    return bytes_t();
  }

  return memory;
}

offsets_t extract_matches(const bytes_t& memory, const bytes_t& searched) {
  offsets_t offsets;

  for (auto it = memory.cbegin();
       (it = search(it, memory.cend(), searched.cbegin(), searched.cend())) != memory.cend();
       ++it) {
    offsets.push_back(distance(memory.cbegin(), it));
  }

  return offsets;
}

void remove_matches_with_different_value(phandle_t process, pid_t pid,
  MemoryBlock& block, const bytes_t& value) {
  auto& matches = block.matches;
  matches.erase(remove_if(matches.begin(), matches.end(), [&](offset_t offset) {
      const auto addr = (block.baseAddress - address_t(0)) + offset;
      const auto memory = read_memory_chunk(process, pid, addr, value.size());
      return memory != value;
    }),
    matches.cend());
}

void lock_process(const pid_t pid) {
  if (ptrace(PTRACE_ATTACH, pid, nullptr, nullptr) < 0) {
    cerr << "Failed to ptrace attach to process." << endl;
    exit(1);
  }
  if (waitpid(pid, NULL, 0) < 0) {
    cerr << "Failed to waitpid on process." << endl;
    exit(1);
  }
}

void unlock_process(const pid_t pid) {
  if (ptrace(PTRACE_DETACH, pid, nullptr, nullptr) < 0) {
    cerr << "Failed to ptrace detach to process." << endl;
    exit(1);
  }
}
