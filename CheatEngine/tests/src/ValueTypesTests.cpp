#include "../../src/ValueTypes.h"

#include "catch.hpp"

TEST_CASE("String can get memory representation") {
  String string;

  SECTION("returns the characters of the string") {
    std::string test = "test";
    auto representation = string.memory_representation(test);
    memory_t expected = { 't', 'e', 's', 't' };
    REQUIRE(representation == expected);
  }
}

TEST_CASE("UnsignedByte can get memory representation") {
  UnsignedByte byte;

  SECTION("returns the right memory") {
    auto representation = byte.memory_representation(0x15);
    memory_t expected = { '\x15' };
    REQUIRE(representation == expected);
  }
}
