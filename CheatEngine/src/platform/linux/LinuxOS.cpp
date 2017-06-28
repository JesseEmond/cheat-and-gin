#include "LinuxOS.h"

#include "LinuxProcess.h"

#include <dirent.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

std::unique_ptr<OS> OS::getOS() { return std::unique_ptr<OS>(new LinuxOS); }


std::vector<pid_t> LinuxOS::getProcesses(const std::string& name) const {
  auto processes = getRunningProcesses();
  processes.erase(std::remove_if(std::begin(processes),
                                 std::end(processes),
                                 [&](pid_t pid) {
                                    return getProcessName(pid) != name;
                                 }),
                  std::end(processes));
  return processes;
}

std::unique_ptr<Process> LinuxOS::open(pid_t pid) {
  return std::unique_ptr<Process>(new LinuxProcess{pid});
}

std::vector<pid_t> LinuxOS::getRunningProcesses() const {
  const auto dir = opendir("/proc");
  if (dir == nullptr) {
    std::cerr << "Failed to open /proc to iterate over running processes."
              << std::endl;
    exit(1);
  }

  std::vector<pid_t> processes;
  dirent* entry;
  while ((entry = readdir(dir)) != nullptr) {
    try {
      pid_t pid = std::stol(entry->d_name);
      processes.push_back(pid);
    } catch(const std::invalid_argument&) {
      // we're only interested in integer pids, so it's fine to fail here
    }
  }

  return processes;
}

std::string LinuxOS::getProcessName(pid_t pid) const {
  std::stringstream procNameFilename;
  procNameFilename << "/proc/" << pid << "/comm";
  std::ifstream procNameFile{procNameFilename.str()};

  if (!procNameFile) {
    std::cerr << "Failed to open comm file for pid "<< pid << std::endl;
    exit(1);
  }

  std::string name;
  procNameFile >> name;
  return name;
}
