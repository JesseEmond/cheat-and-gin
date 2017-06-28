#include "WindowsOS.h"

#include "WindowsProcess.h"

#include <iostream>

std::unique_ptr<OS> OS::getOS() { return std::unique_ptr<OS>(new WindowsOS); }


std::vector<pid_t> WindowsOS::getProcesses(const std::string& name) const {
  vector<pid_t> processes;

  PROCESSENTRY32 entry;
  entry.dwSize = sizeof(PROCESSENTRY32);
  HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

  if (Process32First(snapshot, &entry) == TRUE) {
    while (Process32Next(snapshot, &entry) == TRUE) {
      if (strcmp(entry.szExeFile, name.c_str()) == 0) {
        processes.push_back(entry.th32ProcessID);
      }
    }
  }

  CloseHandle(snapshot);

  return processes;
}

std::unique_ptr<Process> WindowsOS::open(pid_t pid) {
  return std::unique_ptr<Process>(new WindowsProcess{pid});
}
