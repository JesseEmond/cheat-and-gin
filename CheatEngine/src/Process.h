#ifndef PROCESS_H
#define PROCESS_H

#include "platform.h"
#include "MemoryPage.h"

#include <vector>
#include <memory>

/**
 * Represents the process on an OS from which we canread/write from/to its
 * memory.
 *
 * Although we are actually interacting with another process, we conceptually
 * consider reading operations to have no side-effects and mark them as const.
 */
class Process {
protected:
  pid_t pid;

public:
  Process(pid_t pid) : pid{pid} {}

  virtual std::vector<MemoryPage> getCheatablePages() const = 0;
  virtual memory_t read(MemoryPage page) const = 0;
  virtual void write(address_t address, const memory_t& value) = 0;

  virtual ~Process() = default;
};

#endif
