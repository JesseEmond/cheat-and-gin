#include "../../CheatEngine.h"

#include <TlHelp32.h>
#include <Windows.h>

#include <string>
#include <vector>
using namespace std;

phandle_t CheatEngine::openProcess(pid_t id) const {
  return phandle_t();
}

void CheatEngine::closeProcess(phandle_t handle) const {

}

void CheatEngine::keepAddressesWithValue(const value_t& value, value_size_t size) {

}

void CheatEngine::modifyMatchingAddresses(const value_t& value, value_size_t size) const {

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