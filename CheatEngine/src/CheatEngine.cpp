#include "CheatEngine.h"

#include <algorithm>
#include <iterator>
#include <cassert>


/**
 * Search for all the offsets where a value appears in a memory chunk.
 */
offsets_t extract_matches(const memory_t& haystack, const memory_t& needle);


void CheatEngine::search(const memory_t& value) {
  const auto pages = process.getCheatablePages();

  for (const auto& page : pages) {
    const auto memory = process.read(page);
    const auto matches = extract_matches(memory, value);

    if (!matches.empty()) {
      blocks.emplace_back(page, matches);
    }
  }
}

void CheatEngine::narrowDown(const memory_t& value) {
  for (auto& block : blocks) {
    const auto memory = process.read(block.page);

    const auto is_same_value = [&] (offset_t offset) {
      assert(offset + value.size() <= memory.size());
      return std::equal(std::begin(value), std::end(value),
                        std::begin(memory) + offset);
    };

    block.matches.erase(
      std::remove_if(std::begin(block.matches), std::end(block.matches),
                     [&](offset_t offset) { return !is_same_value(offset); }),
      std::end(block.matches));
  }

  const auto is_empty_block = [](const MemoryBlock& block) {
    return block.matches.empty();
  };
  blocks.erase(
    std::remove_if(std::begin(blocks), std::end(blocks), is_empty_block),
    std::end(blocks));
}

void CheatEngine::modify(const memory_t& value) {
  for (const auto& block : blocks) {
    for (auto offset : block.matches) {
      const auto address = block.page.start + offset;
      process.write(address, value);
    }
  }
}


offsets_t extract_matches(const memory_t& haystack, const memory_t& needle) {
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
