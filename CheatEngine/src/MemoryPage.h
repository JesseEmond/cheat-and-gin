#ifndef MEMORY_PAGE_H
#define MEMORY_PAGE_H

#include <cstddef>

typedef char* address_t;
typedef std::size_t size_t;

/**
 * Memory page of the memory space of a process.
 * We read the memory of a process one page at a time.
 */
struct MemoryPage {
  address_t start;
  size_t size;

  MemoryPage(address_t start, size_t size) : start{start}, size{size} {}
};

#endif
