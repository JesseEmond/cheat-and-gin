#ifndef OS_H
#define OS_H

#include "platform.h"
#include "Process.h"

#include <vector>
#include <string>
#include <memory>

struct OS {
  static std::unique_ptr<OS> getOS(); // left to be defined in platform-specific files
  virtual std::vector<pid_t> getProcesses(const std::string& name) const = 0;
  virtual std::unique_ptr<Process> open(pid_t pid) = 0;

  virtual ~OS() = default;
};

#endif
