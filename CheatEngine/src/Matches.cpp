#include "Matches.h"

#include <numeric>


void Matches::add(MemoryPage page, const offsets_t& offsets) {
  if (!offsets.empty()) {
    matches.emplace_back(page, offsets);
  }
}

matches_t::size_type Matches::totalMatches() const {
  return std::accumulate(std::begin(matches), std::end(matches),
                         matches_t::size_type{},
                         [] (matches_t::size_type acc,
                             const PageMatches& matches) {
                          return acc + matches.getOffsets().size();
                         });
}
