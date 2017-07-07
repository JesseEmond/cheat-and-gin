#include "CheatEngine.h"

#include <algorithm>
#include <iterator>
#include <cassert>


/**
 * Search for all the offsets where a value appears in a memory chunk.
 */
offsets_t matching_offsets(const memory_t& haystack, const memory_t& needle);


Matches CheatEngine::search(const memory_t& value) const {
  Matches matches;
  const auto pages = process.getCheatablePages();

  for (const auto& page : pages) {
    const auto memory = process.read(page);
    const auto offsets = matching_offsets(memory, value);
    matches.add(page, offsets);
  }

  return matches;
}

Matches CheatEngine::narrowDown(const Matches& matches,
                                const memory_t& value) const {
  Matches narrowed;

  for (const auto& match : matches.getPageMatches()) {
    const auto memory = process.read(match.getPage());

    const auto is_same_value = [&] (offset_t offset) {
      assert(offset + value.size() <= memory.size());
      return std::equal(std::begin(value), std::end(value),
                        std::begin(memory) + offset);
    };

    offsets_t offsets;
    std::copy_if(std::begin(match.getOffsets()), std::end(match.getOffsets()),
                 std::back_inserter(offsets),
                 is_same_value);
    narrowed.add(match.getPage(), offsets);
  }

  return narrowed;
}

void CheatEngine::modify(const Matches& matches, const memory_t& value) {
  for (const auto& match : matches.getPageMatches()) {
    for (auto offset : match.getOffsets()) {
      const auto address = match.getPage().start + offset;
      process.write(address, value);
    }
  }
}

bool CheatEngine::doneSearching(const Matches& matches) const {
  return matches.totalMatches() <= 1;
}


offsets_t matching_offsets(const memory_t& haystack, const memory_t& needle) {
  offsets_t offsets;

  for (auto it = haystack.cbegin();
       (it = std::search(it, haystack.cend(), needle.cbegin(), needle.cend()))
       != haystack.cend();
       ++it) {
    const auto offset = std::distance(haystack.cbegin(), it);
    offsets.push_back(offset);
  }

  return offsets;
}
