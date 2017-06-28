#ifndef LINUX_PROCESS_H
#define LINUX_PROCESS_H

#include "../../Process.h"

class LinuxProcess : public Process {
  phandle_t handle;

public:
  LinuxProcess(pid_t pid);
  ~LinuxProcess();

  std::vector<MemoryPage> getCheatablePages() override;
  memory_t read(MemoryPage page) override;
  void write(address_t address, const memory_t& value) override;
};

#endif
