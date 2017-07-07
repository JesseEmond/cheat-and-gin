#ifndef FAKE_PROCESS_H
#define FAKE_PROCESS_H

#include "../../src/Process.h"

#include <utility>


typedef std::pair<address_t, memory_t> written;


class no_more_reads_stored {};


class FakeProcess : public Process {
  mutable std::vector<memory_t>::size_type readIndex;
  std::vector<written> writes;

public:
  // to provide when testing
  std::vector<MemoryPage> pages;
  std::vector<memory_t> reads;


  FakeProcess(std::vector<MemoryPage> pages = {},
              std::vector<memory_t> reads = {})
    : Process{0},
      pages{pages}, reads{reads},
      writes{}, readIndex{0} {}

  std::vector<MemoryPage> getCheatablePages() const override { return pages; }
  memory_t read(MemoryPage) const override {
    if (readIndex >= reads.size()) throw no_more_reads_stored{};
    return reads[readIndex++];
  }
  void write(address_t address, const memory_t& value) override {
    writes.emplace_back(address, value);
  }

  const std::vector<written>& getWrites() const { return writes; }
};


#endif
