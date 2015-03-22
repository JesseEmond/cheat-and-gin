#ifndef MEMORY_BLOCK_H
#define MEMORY_BLOCK_H

#include <vector>

typedef char* address_t;
typedef unsigned int block_size_t;
typedef unsigned int offset_t;
typedef std::vector<offset_t> offsets_t;

struct MemoryBlock
{
  address_t baseAddress;
  block_size_t size;
  offsets_t matches;
};

#endif
