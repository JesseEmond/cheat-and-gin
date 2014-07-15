#include "../../CheatEngine.h"

#include <TlHelp32.h>
#include <Windows.h>

#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <cassert>
using namespace std;

// Declarations
void write_address(phandle_t process, CheatEngine::address_t address, CheatEngine::value_t value, CheatEngine::value_size_t size);
DWORD get_chunk_size_to_read();
bool can_modify_page(const MEMORY_BASIC_INFORMATION& page);


// Definitions
phandle_t CheatEngine::openProcess(pid_t id) const {
  auto handle = OpenProcess(PROCESS_ALL_ACCESS, false, id);

  if (handle == nullptr) {
  	cerr << "couldn't open process." << endl;
  	exit(1);
  }

  return handle;
}

void CheatEngine::closeProcess(phandle_t handle) const {
  CloseHandle(handle);
}

void CheatEngine::keepAddressesWithValue(const value_t& value, value_size_t size) {
	addresses_t newAddresses;
}

void CheatEngine::addAddressesWithValue(const value_t& value, value_size_t size) {
  assert(size > 0);

  MEMORY_BASIC_INFORMATION info;
  for (address_t address = nullptr;
    VirtualQueryEx(m_process, address, &info, sizeof(info)) == sizeof(info);
    address += info.RegionSize) {
    if (can_modify_page(info)) {
      vector<char> chunk(info.RegionSize);
      SIZE_T bytesRead;
      ReadProcessMemory(m_process, address, chunk.data(), info.RegionSize, &bytesRead);
      chunk.resize(bytesRead);

      for (vector<char>::size_type i = 0; i <= chunk.size() - size; ++i) {
        address_t chunkPtr = chunk.data() + i;
        address_t valuePtr = value;

        if (equal(valuePtr, valuePtr + size, valuePtr)) {
          address_t matchingAddress = address + i;

          m_addresses.emplace_back(matchingAddress);
        }
      }
    }
  }
}

void CheatEngine::modifyMatchingAddresses(const value_t& value, value_size_t size) const {
  for_each(begin(m_addresses), end(m_addresses), [&](address_t address) {
    write_address(m_process, address, value, size);
  });
}

vector<pid_t> CheatEngine::getProcessesWithName(const std::string& name) {
  vector<pid_t> processes;

	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry) == TRUE) {
		while (Process32Next(snapshot, &entry) == TRUE) {
			if (strcmp(entry.szExeFile, name.c_str()) == 0) {
				processes.emplace_back(entry.th32ProcessID);
			}
		}
	}

	CloseHandle(snapshot);

	return processes;
}

void write_address(phandle_t process, CheatEngine::address_t address, CheatEngine::value_t value, CheatEngine::value_size_t size) {
	if (!WriteProcessMemory(process, address, value, size, nullptr))
		cerr << "error while trying to write on the other process" << endl;
}

DWORD get_chunk_size_to_read() {
  SYSTEM_INFO system;
  GetSystemInfo(&system);

  return system.dwPageSize;
}

bool can_modify_page(const MEMORY_BASIC_INFORMATION& page) {
	return page.State   == MEM_COMMIT  &&
				 page.Type    == MEM_PRIVATE &&
				 page.Protect == PAGE_READWRITE;
}