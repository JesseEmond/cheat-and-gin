#ifndef LINUX_PROCESS_H
#define LINUX_PROCESS_H

#include "../../Process.h"

struct LinuxProcess : public Process {
  LinuxProcess(pid_t pid) : Process{pid} {}

  std::vector<MemoryPage> getCheatablePages() const override;
  memory_t read(MemoryPage page) const override;
  void write(address_t address, const memory_t& value) override;
};

#endif
