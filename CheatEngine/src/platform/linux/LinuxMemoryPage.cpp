#include "LinuxMemoryPage.h"

#include <regex>
#include <string>

LinuxMemoryPage parse_memory_map_line(const std::string& line) {
  static const std::regex pattern(
    "([0-9A-Fa-f]+)-([0-9A-Fa-f]+) ([-r])([-w])[-x]([sp]).*");

  std::smatch matches;
  std::regex_match(line, matches, pattern);

  if (matches.size() != 6) throw invalid_memory_map_line{};

  address_t start;
  address_t end;

  try {
    start = (address_t)std::stoull(matches[1], nullptr, 16);
    end = (address_t)std::stoull(matches[2], nullptr, 16);
  } catch (const std::invalid_argument&) {
    throw invalid_memory_map_line{};
  }

  const auto read = matches[3] == "r";
  const auto write = matches[4] == "w";
  const auto shared = matches[5] == "s";

  return LinuxMemoryPage{start, static_cast<size_t>(end - start),
                         read, write, shared};
}

bool LinuxMemoryPage::cheatable() const {
  return read && write && !shared;
}
