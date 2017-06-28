#ifndef WINDOWS_OS_H
#define WINDOWS_OS_H

#include "../../OS.h"

struct WindowsOS : public OS {
  std::vector<pid_t> getProcesses(const std::string& name) const override;
  std::unique_ptr<Process> open(pid_t pid) override;
};

#endif

