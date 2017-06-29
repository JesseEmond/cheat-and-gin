#include "../../src/platform/linux/LinuxMemoryPage.h"

#include "catch.hpp"

#include <string>

TEST_CASE("LinuxMemoryMap can parse a /proc/[pid]/maps line", "[linux]") {
  SECTION("can parse a valid line") {
    const auto line = "7f14b76c1000-7f14b78c1000 rw-s 00000000 00:18 1886948                    /dev/shm/.org.chromium.Chromium.iQ0oVc (deleted)";
    const auto page = parse_memory_map_line(line);

    REQUIRE((long int)page.start == 0x7f14b76c1000);
    REQUIRE(page.size == (0x7f14b78c1000 - 0x7f14b76c1000));
    REQUIRE(page.read);
    REQUIRE(page.write);
    REQUIRE(page.shared);
  }

  SECTION("fails to parse an unrelated string") {
    REQUIRE_THROWS_AS(parse_memory_map_line("unrelated string."),
                      invalid_memory_map_line);
  }

  SECTION("fails to parse a line with an invalid start address") {
    const auto line = "invalid-7f14b78c1000 rw-s 00000000 00:18 1886948                    /dev/shm/.org.chromium.Chromium.iQ0oVc (deleted)";
    REQUIRE_THROWS_AS(parse_memory_map_line(line), invalid_memory_map_line);
  }

  SECTION("fails to parse a line with an invalid end address") {
    const auto line = "7f14b78c1000-invalid rw-s 00000000 00:18 1886948                    /dev/shm/.org.chromium.Chromium.iQ0oVc (deleted)";
    REQUIRE_THROWS_AS(parse_memory_map_line(line), invalid_memory_map_line);
  }

  SECTION("does not fail with huge offsets") {
    const auto line = "ffffffffff600000-ffffffffff601000 r-xp 00000000 00:00 0                  [vsyscall]";
    REQUIRE_NOTHROW(parse_memory_map_line(line));
  }
}
