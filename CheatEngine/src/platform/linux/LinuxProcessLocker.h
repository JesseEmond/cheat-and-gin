#ifndef LINUX_PROCESS_LOCKER_H
#define LINUX_PROCESS_LOCKER_H

#include "../../OS.h"

struct LinuxProcessLocker {
  pid_t pid;

  LinuxProcessLocker(pid_t pid);

  ~LinuxProcessLocker();
};

#endif
