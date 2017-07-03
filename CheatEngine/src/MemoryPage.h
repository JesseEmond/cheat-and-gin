#ifndef MEMORY_PAGE_H
#define MEMORY_PAGE_H

#include <cstddef>
#include <vector>

typedef unsigned char byte_t;
typedef std::vector<byte_t> memory_t;
typedef byte_t* address_t;
typedef memory_t::size_type size_t;

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
