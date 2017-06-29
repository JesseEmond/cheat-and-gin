#include "../../src/PageMatches.h"

#include "catch.hpp"

TEST_CASE("PageMatches can be initialized") {
  SECTION("accepts non-empty offsets") {
    MemoryPage page{nullptr, 0};
    offsets_t offsets{ 1, 2, 3 };
    REQUIRE_NOTHROW(PageMatches(page, offsets));
  }

  SECTION("throws with empty offsets") {
    MemoryPage page{nullptr, 0};
    offsets_t offsets{};
    REQUIRE_THROWS_AS(PageMatches(page, offsets), no_matches);
  }
}

