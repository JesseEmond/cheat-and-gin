#ifndef PAGE_MATCHES_H
#define PAGE_MATCHES_H

#include "MemoryPage.h"

#include <vector>


typedef unsigned int offset_t;
typedef std::vector<offset_t> offsets_t;


class no_matches {};


/**
  * Represents found matches within a memory page.
  */
class PageMatches {
  MemoryPage page;
  offsets_t offsets;

public:
  /**
   * If no offsets are provided, a no_matches exception is thrown.
   */
  PageMatches(MemoryPage page, const offsets_t& offsets)
    : page{page}, offsets{offsets} {
      if (offsets.empty()) throw no_matches{};
    }

  const MemoryPage& getPage() const { return page; }
  const offsets_t& getOffsets() const { return offsets; }
};

#endif
