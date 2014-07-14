#include "../../CheatEngine.h"

#include <TlHelp32.h>
#include <Windows.h>

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

// Declarations
void write_address(phandle_t process, CheatEngine::address_t address, CheatEngine::value_t value, CheatEngine::value_size_t size);


// Definitions
phandle_t CheatEngine::openProcess(pid_t id) const {
  return OpenProcess(PROCESS_ALL_ACCESS, false, id);
}

void CheatEngine::closeProcess(phandle_t handle) const {
  CloseHandle(handle);
}

void CheatEngine::keepAddressesWithValue(const value_t& value, value_size_t size) {

}

void CheatEngine::addAddressesWithValue(const value_t& value, value_size_t size) {

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