#include "LinuxProcess.h"

#include "LinuxMemoryPage.h"
#include "LinuxProcessLocker.h"

#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>

#include <sstream>
#include <fstream>
#include <iostream>
#include <string>

LinuxProcess::LinuxProcess(pid_t pid) : Process{pid} {
  std::stringstream memFilename;
  memFilename << "/proc/" << pid << "/mem";

  handle = open(memFilename.str().c_str(), O_RDONLY);
  if (handle < 0) {
    std::cerr << "Failed to open process memory file. "
              << "Do you have the required rights?" << std::endl;
    exit(1);
  }
}

LinuxProcess::~LinuxProcess() {
  if (close(handle) < 0) {
    // we're in a dtor, nothing else we can do except report and ignore
    std::cerr << "Failed to close process memory file." << std::endl;
  }
}

std::vector<MemoryPage> LinuxProcess::getCheatablePages() {
  LinuxProcessLocker lock{pid};

  std::stringstream memoryMapFilename;
  memoryMapFilename << "/proc/" << pid << "/maps";
  std::ifstream memMap(memoryMapFilename.str());

  if (!memMap) {
    std::cerr << "Failed to open memory map file for pid " << pid << std::endl;
    exit(1);
  }

  std::vector<MemoryPage> pages;
  std::string line;
  while (getline(memMap, line)) {
    try {
      const auto page = parse_memory_map_line(line);

      if (page.cheatable()) {
        pages.push_back(page);
      }
    } catch (const invalid_memory_map_line&) {
      std::cerr << "Failed to parse the memory map line \""
                << line << "\"." << std::endl;
      exit(1);
    }
  }

  return pages;
}

memory_t LinuxProcess::read(MemoryPage page) {
  LinuxProcessLocker lock{pid};

  memory_t memory(page.size);
  const long start = reinterpret_cast<long>(page.start);
  const auto offset = lseek(handle, start, SEEK_SET);

  if (offset != start) {
    std::cerr << "Failed to seek in memory fiile of process." << std::endl;
    exit(1);
  }

  if (::read(handle, memory.data(), page.size) < 0) {
    std::cerr << "Error while reading memory from memory file of process."
              << std::endl;
    exit(1);
  }

  return memory;
}

void LinuxProcess::write(address_t address, const memory_t& value) {
  LinuxProcessLocker lock{pid};

  // iovec takes non-const char* (even though we call process_vm_writev, which
  // doesn't modify our local memory), so we need to cast away the constness.
  void* localAddress = const_cast<byte_t*>(value.data());
  const iovec local = { localAddress, value.size() };
  const iovec remote = { address, value.size() };

  const auto written = process_vm_writev(pid, &local, 1, &remote, 1, 0);

  if (written < 0 || written != value.size()) {
    std::cerr << "Failed to write memory to target process." << std::endl;
  }
}
