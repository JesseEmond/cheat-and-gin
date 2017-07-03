#ifndef VALUE_TYPE_H
#define VALUE_TYPE_H

#include "MemoryPage.h"

#include <string>

struct ValueType {
  const std::string name;

  ValueType(const std::string& name) : name{name} {}
  virtual memory_t ask_value() const = 0;
  virtual ~ValueType() = default;
};

#endif
