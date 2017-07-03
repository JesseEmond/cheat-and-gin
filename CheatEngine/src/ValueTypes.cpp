#include "ValueTypes.h"

#include "helper.h"

#include <iterator>
#include <sstream>
#include <cassert>


const std::unique_ptr<ValueType> TYPES[] = {
  std::unique_ptr<ValueType>{new UnsignedByte},
  std::unique_ptr<ValueType>{new Character},
  std::unique_ptr<ValueType>{new Int16},
  std::unique_ptr<ValueType>{new Int32},
  std::unique_ptr<ValueType>{new Int64},
  std::unique_ptr<ValueType>{new Float},
  std::unique_ptr<ValueType>{new Double},
};
const std::size_t TYPES_COUNT = std::distance(std::begin(TYPES),
                                              std::end(TYPES));


const ValueType& ask_for_value_type() {
  static const char FIRST = 'A';
  std::stringstream query, error;
  for (std::size_t i = 0; i < TYPES_COUNT; ++i) {
    query << static_cast<char>(FIRST + i) << ") "
          << TYPES[i]->name << std::endl;
  }
  query << "Value type";
  error << "Choice must be between '" << FIRST << "' and '"
        << static_cast<char>(FIRST + TYPES_COUNT - 1) << "'";

  const char choice = ask_for<char>(query.str(), error.str(), [](char c) {
    return FIRST <= c && c < FIRST + TYPES_COUNT;
  });

  const int index = choice - FIRST;
  assert(0 <= index && index < TYPES_COUNT);
  return *TYPES[index];
}
