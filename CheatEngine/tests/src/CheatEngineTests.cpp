#include "../../src/CheatEngine.h"

#include "FakeProcess.h"

#include "catch.hpp"

#include <string>


TEST_CASE("CheatEngine can search for matches") {
  FakeProcess process;
  CheatEngine engine{process};
  Matches matches;
  const std::string str = "this is a test.";
  memory_t memory{std::begin(str), std::end(str)};
  process.pages = { MemoryPage(memory.data(), memory.size()) };
  process.reads = { memory };

  SECTION("does not find a match when the value is missing") {
    const auto matches = engine.search({ 't', 'e', 's', 's' });

    REQUIRE(matches.getPageMatches().empty());
  }

  SECTION("does find a match when the value present") {
    const auto matches = engine.search({ 'i', 's' });

    REQUIRE(matches.getPageMatches().size() == 1);
    const auto match = matches.getPageMatches().front();
    const offsets_t offsets = {2, 5};
    REQUIRE(match.getOffsets() == offsets);
  }
}

TEST_CASE("CheatEngine can narrow down search matches") {
  FakeProcess process;
  CheatEngine engine{process};
  Matches matches;

  const std::string str = "this is a test.";
  memory_t memory{std::begin(str), std::end(str)};

  process.pages = { MemoryPage(memory.data(), memory.size()) };
  process.reads = { memory };

  SECTION("does not keep the matches when they no longer match") {
    matches.add(process.pages.front(), offsets_t{0});
    const auto narrowed = engine.narrowDown(matches, { 't', 'h', 'a', 't' });

    REQUIRE(narrowed.getPageMatches().empty());
  }

  SECTION("keeps the matches that still match") {
    matches.add(process.pages.front(), offsets_t{0, 5});
    const auto narrowed = engine.narrowDown(matches, { 't', 'h', 'i', 's' });

    REQUIRE(narrowed.getPageMatches().size() == 1);
    const auto match = narrowed.getPageMatches().front();
    const offsets_t offsets = {0};
    REQUIRE(match.getOffsets() == offsets);
  }
}

TEST_CASE("CheatEngine can modify matches") {
  FakeProcess process;
  CheatEngine engine{process};
  Matches matches;

  SECTION("does not write when there are no matches") {
    engine.modify(matches, {});

    REQUIRE(process.getWrites().empty());
  }

  SECTION("writes to all the matches at the right address") {
    std::string str = "this is memory.";
    memory_t memory(std::begin(str), std::end(str));
    MemoryPage page{memory.data(), memory.size()};
    offsets_t offsets = { 2, 5 }; // matches on the 'i's
    matches.add(page, offsets);
    memory_t newval = { 'o' };

    engine.modify(matches, newval);

    REQUIRE(process.getWrites().size() == 2);
    REQUIRE(process.getWrites()[0].first == page.start + offsets[0]);
    REQUIRE(process.getWrites()[0].second == newval);
    REQUIRE(process.getWrites()[1].first == page.start + offsets[1]);
    REQUIRE(process.getWrites()[1].second == newval);
  }
}
