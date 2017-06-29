#ifndef CHEAT_ENGINE_H
#define CHEAT_ENGINE_H

#include "Process.h"
#include "Matches.h"
#include "platform.h"

#include <string>
#include <vector>


class CheatEngine
{
  Process& process;

public:
  CheatEngine(Process& process) : process{process} {}

  /**
   * Search the process' memory to find occurences of a certain value.
   */
  Matches search(const memory_t& value);

  /**
   * Narrow down the address matches based on which ones now have the specified
   * value.
   */
  Matches narrowDown(const Matches& matches, const memory_t& value);

  /**
   * Modify the matching addresses with the given value.
   */
  void modify(const Matches& matches, const memory_t& value);
};

#endif
