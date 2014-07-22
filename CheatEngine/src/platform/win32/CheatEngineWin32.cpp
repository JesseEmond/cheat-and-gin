#include "../../CheatEngine.h"

#include <TlHelp32.h>
#include <Windows.h>

#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <cassert>
#include <utility>
using namespace std;

typedef std::vector<unsigned char> chunk_t;

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

void CheatEngine::addAddressesWithValue(const value_t& value, value_size_t size) {
  assert(size > 0);

  MEMORY_BASIC_INFORMATION info;
  for (address_t address = nullptr;
    VirtualQueryEx(m_process, address, &info, sizeof(info)) == sizeof(info);
    address = static_cast<address_t>(info.BaseAddress) + info.RegionSize) {
    if (can_modify_page(info)) {
      chunk_t chunk(info.RegionSize);
      if (!ReadProcessMemory(m_process, address, chunk.data(), info.RegionSize, nullptr)) {
        cerr << "couldn't properly read process memory. " << GetLastError() << endl;
      }
      assert(!chunk.empty());

      for (chunk_t::size_type i = 0; i <= chunk.size() - size; ++i) {
        address_t chunkPtr = chunk.data() + i;
        address_t valuePtr = value;

        if (equal(chunkPtr, chunkPtr + size, valuePtr)) {
          address_t matchingAddress = static_cast<address_t>(info.BaseAddress) + i;

          m_addresses.insert(make_pair(address, matchingAddress));
        }
      }
    }
  }
}

void CheatEngine::keepAddressesWithValue(const value_t& value, value_size_t size) {
  assert(!m_addresses.empty());

  addresses_t keptAddresses;

  // Iterate over all the memory pages
  for (auto currentPageIt = begin(m_addresses);
    currentPageIt != end(m_addresses);) {
    // get the range of addresses where we found a value (within that page)
    const auto pageRange = m_addresses.equal_range(currentPageIt->first);

    // read that memory page again (if we can still read it)
    MEMORY_BASIC_INFORMATION info;
    const auto len = VirtualQueryEx(m_process, currentPageIt->first, &info, sizeof(info));
    assert(len == sizeof(info));
    if (can_modify_page(info)) {
      // read the chunk of memory where we had addresses before
      chunk_t chunk(info.RegionSize);
      if (!ReadProcessMemory(m_process, currentPageIt->first, chunk.data(), info.RegionSize, nullptr)) {
        cerr << "couldn't properly read process memory. " << GetLastError() << endl;
      }
      assert(!chunk.empty());

      // go through all the addresses in the memory page
      for (auto addrIt = pageRange.first; addrIt != pageRange.second; ++addrIt) {
        assert(addrIt->second >= addrIt->first);
        const size_t index = addrIt->second - addrIt->first;
        assert(index + size <= chunk.size());
        const auto addrInChunk = chunk.data() + index;

        // if the address still matches our value, keep it.
        if (equal(addrInChunk, addrInChunk + size, value)) {
          keptAddresses.insert(make_pair(addrIt->first, addrIt->second));
        }
      }
    }

    currentPageIt = pageRange.second;
  }

  // keep the new addresses
  m_addresses = keptAddresses;
}

void CheatEngine::modifyMatchingAddresses(const value_t& value, value_size_t size) const {
  for_each(begin(m_addresses), end(m_addresses), [&](std::pair<address_t, address_t> pageAddress) {
    write_address(m_process, pageAddress.second, value, size);
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