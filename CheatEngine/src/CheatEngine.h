#ifndef CHEAT_ENGINE_H
#define CHEAT_ENGINE_H

#include "Process.h"
#include "MemoryBlock.h"
#include "platform.h"

#include <string>
#include <vector>


typedef std::vector<MemoryBlock> blocks_t;


class CheatEngine
{
  Process& process;
  blocks_t blocks;

public:
  const blocks_t& getMatchingBlocks() const { return blocks; }

  CheatEngine(Process& process) : process{process}, blocks{} {}

  /**
   * Search the process' memory to find occurences of a certain value.
   */
  void search(const memory_t& value);

  /**
   * Narrow down the address matches based on which ones have the specified
   * value.
   */
  void narrowDown(const memory_t& value);

  /**
   * Modify the matching addresses with the given value.
   */
  void modify(const memory_t& value);
};

#endif
