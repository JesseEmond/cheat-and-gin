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
void write_address(phandle_t process, address_t address, CheatEngine::value_t value, CheatEngine::value_size_t size);
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
		address = static_cast<unsigned char*>(info.BaseAddress) + info.RegionSize) {
		if (can_modify_page(info)) {
			chunk_t chunk(info.RegionSize);
			if (!ReadProcessMemory(m_process, address, chunk.data(), info.RegionSize, nullptr)) {
				cerr << "couldn't properly read process memory. " << GetLastError() << endl;
			}
			assert(!chunk.empty());

			offsets_t matches;
			for (chunk_t::size_type i = 0; i <= chunk.size() - size; ++i) {
        if (std::equal(chunk.cbegin() + i, chunk.cbegin() + i + size, value.cbegin())) {
					matches.push_back(i);
				}
			}
			if (!matches.empty()) {
				MemoryBlock block;
				block.matches = matches;
				block.baseAddress = info.BaseAddress;
				block.size = info.RegionSize;
				m_blocks.push_back(block);
			}
		}
	}
}

void CheatEngine::keepAddressesWithValue(const value_t& value, value_size_t size) {
	assert(!m_blocks.empty());

	for (auto blockIt = begin(m_blocks); blockIt != end(m_blocks);) {
		MEMORY_BASIC_INFORMATION info;
		const auto len = VirtualQueryEx(m_process, blockIt->baseAddress, &info, sizeof(info));
		assert(len == sizeof(info));

		if (can_modify_page(info)) {
			chunk_t chunk(blockIt->size);
			if (!ReadProcessMemory(m_process, blockIt->baseAddress, chunk.data(), blockIt->size, nullptr)) {
				cerr << "couldn't properly read process memory. " << GetLastError() << endl;
			}
			assert(!chunk.empty());

			// from the offsets that matched before within that page, only keep the ones that still match
			offsets_t stillMatches;
			for (auto offsetIt = blockIt->matches.cbegin(); offsetIt != blockIt->matches.cend(); ++offsetIt) {
				assert(*offsetIt + size <= chunk.size());
        if (std::equal(chunk.cbegin() + *offsetIt, chunk.cbegin() + *offsetIt + size, value.cbegin())) {
					stillMatches.push_back(*offsetIt);
				}
			}
			blockIt->matches = stillMatches;

			// memory page no longer has matching addresses: drop it
			if (blockIt->matches.empty()) {
				blockIt = m_blocks.erase(blockIt);
			}
			else {
				++blockIt;
			}
		}
	}
}

void CheatEngine::modifyMatchingAddresses(const value_t& value, value_size_t size) const {
  for_each(begin(m_blocks), end(m_blocks), [&](const MemoryBlock& block) {
    for_each(begin(block.matches), end(block.matches), [&](const offset_t offset) {
      write_address(m_process, static_cast<unsigned char*>(block.baseAddress) + offset, value, size);
    });
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

void write_address(phandle_t process, address_t address, CheatEngine::value_t value, CheatEngine::value_size_t size) {
	if (!WriteProcessMemory(process, address, value.data(), size, nullptr))
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