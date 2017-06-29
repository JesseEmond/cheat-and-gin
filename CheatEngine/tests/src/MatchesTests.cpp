#include "../../src/Matches.h"

#include "catch.hpp"

TEST_CASE("PageMatches can add matches") {
  Matches matches;
  MemoryPage page{nullptr, 0};

  SECTION("can add non-empty matches") {
    offsets_t offsets{1, 2, 3};
    matches.add(page, offsets);

    const auto pageMatches = matches.getPageMatches();
    REQUIRE(pageMatches.size() == 1);
    REQUIRE(pageMatches[0].getOffsets() == offsets);
  }

  SECTION("ignores adds with empty offsets") {
    offsets_t offsets;
    matches.add(page, offsets);

    REQUIRE(matches.getPageMatches().empty());
  }
}

TEST_CASE("PageMatches can count matches") {
  Matches matches;

  SECTION("returns 0 on empty matches") {
    Matches matches;
    REQUIRE(matches.totalMatches() == 0);
  }

  SECTION("returns the offsets count of a page for one page") {
    MemoryPage page{nullptr, 0};
    offsets_t offsets{ 1, 2, 3 };

    matches.add(page, offsets);

    REQUIRE(matches.totalMatches() == 3);
  }

  SECTION("returns the sum of offsets for multiple pages") {
    MemoryPage page1{nullptr, 0};
    offsets_t offsets1{ 1, 2, 3 };
    MemoryPage page2{nullptr, 1};
    offsets_t offsets2{ 4, 5 };

    matches.add(page1, offsets1);
    matches.add(page2, offsets2);

    REQUIRE(matches.totalMatches() == 5);
  }
}
