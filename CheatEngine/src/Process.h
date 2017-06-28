#ifndef PROCESS_H
#define PROCESS_H

#include "platform.h"
#include "MemoryPage.h"

#include <vector>
#include <memory>

typedef unsigned char byte_t;
typedef std::vector<byte_t> memory_t;

/**
 * Represents the process on an OS from which we canread/write from/to its
 * memory.
 */
class Process {
protected:
  pid_t pid;

public:
  Process(pid_t pid) : pid{pid} {}

  virtual std::vector<MemoryPage> getCheatablePages() = 0;
  virtual memory_t read(MemoryPage page) = 0;
  virtual void write(address_t address, const memory_t& value) = 0;

  virtual ~Process() = default;
};

#endif
