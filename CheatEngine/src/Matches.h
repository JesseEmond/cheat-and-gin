#ifndef MATCHES_H
#define MATCHES_H

#include "PageMatches.h"

#include <vector>


typedef std::vector<PageMatches> matches_t;


/**
 * Represents all the memory matches on a process.
 */
class Matches {
  matches_t matches;

public:
  const matches_t& getPageMatches() const { return matches; }

  void add(MemoryPage page, const offsets_t& offsets);
  matches_t::size_type totalMatches() const;
  bool any() const { return totalMatches() > 0; }
};

#endif
