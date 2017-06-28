#include "LinuxProcessLocker.h"

#include <sys/ptrace.h>
#include <sys/wait.h>

#include <iostream>

LinuxProcessLocker::LinuxProcessLocker(pid_t pid) : pid{pid} {
  if (ptrace(PTRACE_ATTACH, pid, nullptr, nullptr) < 0) {
    std::cerr << "Failed to ptrace attach to process." << std::endl;
    exit(1);
  }
  if (waitpid(pid, nullptr, 0) < 0) {
    std::cerr << "Failed to waitpid on process." << std::endl;
    exit(1);
  }
}
LinuxProcessLocker::~LinuxProcessLocker() {
  if (ptrace(PTRACE_DETACH, pid, nullptr, nullptr) < 0) {
    std::cerr << "Failed to ptrace detach from process." << std::endl;
  }
}
