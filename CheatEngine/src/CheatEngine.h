#ifndef CHEAT_ENGINE_H
#define CHEAT_ENGINE_H

#include "Process.h"
#include "Matches.h"
#include "platform.h"

#include <string>
#include <vector>


/**
 * Core engine that takes care of searching for a particular value in another
 * process' memory and changing it.
 */
class CheatEngine
{
  Process& process;

public:
  CheatEngine(Process& process) : process{process} {}

  /**
   * Search the process' memory to find occurences of a certain value.
   */
  Matches search(const memory_t& value) const;

  /**
   * Narrow down the address matches based on which ones now have the specified
   * value.
   */
  Matches narrowDown(const Matches& matches, const memory_t& value) const;

  /**
   * Modify the matching addresses with the given value.
   */
  void modify(const Matches& matches, const memory_t& value);

  /**
   * Indicates if we are sure that we are done searching for an address based
   * on the matches so far (no more matches or only one left).
   */
  bool doneSearching(const Matches& matches) const;
};

#endif
