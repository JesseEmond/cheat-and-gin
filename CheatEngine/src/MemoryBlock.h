#ifndef MEMORY_BLOCK_H
#define MEMORY_BLOCK_H

#include "MemoryPage.h"

#include <vector>

typedef unsigned int offset_t;
typedef std::vector<offset_t> offsets_t;

struct MemoryBlock
{
  MemoryPage page;
  offsets_t matches;

  MemoryBlock(const MemoryPage& page, const offsets_t& matches)
    : page{page}, matches{matches} {}
};

#endif
