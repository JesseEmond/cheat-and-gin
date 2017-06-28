#ifndef LINUX_MEMORY_PAGE_H
#define LINUX_MEMORY_PAGE_H

#include "../../MemoryPage.h"

#include <string>

struct LinuxMemoryPage : public MemoryPage {
  bool read, write, shared;

  LinuxMemoryPage(address_t start, size_t size,
                  bool read, bool write, bool shared)
    : MemoryPage(start, size),
      read{read}, write{write}, shared{shared} {
    }

  bool cheatable() const;
};

/**
 * Parse a line from /proc/[pid]/maps
 */
LinuxMemoryPage parse_memory_map_line(const std::string& line);

class invalid_memory_map_line {};

#endif
