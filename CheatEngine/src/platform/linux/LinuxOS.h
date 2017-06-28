#ifndef LINUX_OS_H
#define LINUX_OS_H

#include "../../OS.h"

struct LinuxOS : public OS {
  std::vector<pid_t> getProcesses(const std::string& name) const override;
  std::unique_ptr<Process> open(pid_t pid) override;
  virtual ~LinuxOS() = default;

protected:
  virtual std::vector<pid_t> getRunningProcesses() const;
  virtual std::string getProcessName(pid_t pid) const;
};

#endif
