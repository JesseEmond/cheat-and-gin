#include "FakeProcess.h"

#include "catch.hpp"


TEST_CASE("FakeProcess can hold pages") {
  std::vector<MemoryPage> pages = {MemoryPage{nullptr, 0},
                                   MemoryPage{nullptr, 1}};
  FakeProcess process{pages};
  REQUIRE(process.getCheatablePages().size() == 2);
  REQUIRE(process.getCheatablePages()[0].size == 0);
  REQUIRE(process.getCheatablePages()[1].size == 1);
}

TEST_CASE("FakeProcess can provide reads") {
  MemoryPage page{nullptr, 0};
  FakeProcess process;

  SECTION("can read once") {
    memory_t memory = { 'a', 'b' };
    process.reads = { memory };

    REQUIRE(process.read(page) == memory);
  }

  SECTION("can read multiple times") {
    memory_t memory1 = { 'a', 'b' };
    memory_t memory2 = { 'c', 'd', 'e' };
    process.reads = { memory1, memory2 };

    REQUIRE(process.read(page) == memory1);
    REQUIRE(process.read(page) == memory2);
  }

  SECTION("fails after too many reads") {
    memory_t memory = { 'a' };
    process.reads = { memory };

    REQUIRE(process.read(page) == memory);
    REQUIRE_THROWS_AS(process.read(page), no_more_reads_stored);
  }

  SECTION("fails when no reads set") {
    REQUIRE_THROWS_AS(process.read(page), no_more_reads_stored);
  }
}

TEST_CASE("FakeProcess stores writes") {
  FakeProcess process;

  SECTION("nothing stored without calling write") {
    REQUIRE(process.getWrites().empty());
  }

  SECTION("can store a single write") {
    memory_t memory = { 'a', 'b' };

    process.write(nullptr, memory);

    REQUIRE(process.getWrites().size() == 1);
    REQUIRE(process.getWrites()[0].first == nullptr);
    REQUIRE(process.getWrites()[0].second == memory);
  }

  SECTION("can store multiple writes") {
    memory_t memory1 = { 'a', 'b' };
    memory_t memory2 = { 'c', 'd', 'e' };

    process.write(nullptr, memory1);
    process.write(nullptr, memory2);

    REQUIRE(process.getWrites().size() == 2);
    REQUIRE(process.getWrites()[0].second == memory1);
    REQUIRE(process.getWrites()[1].second == memory2);
  }
}
