#include "ValueType.h"

#include "helper.h"

#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <cassert>
#include <cstdint>
using namespace std;

// Declarations
std::vector<std::pair<ValueType, std::string>> make_value_types();
template <class T>
std::vector<unsigned char> ask_value_for(const std::string& valueName, const std::string& error);

class unsupported_type_exception {};


// Globals
const auto VALUE_TYPES = make_value_types();


// Definitions
std::size_t value_type_size(ValueType type) {
  switch (type) {
    case ValueTypes::byte_t: return sizeof(int8_t);
    case ValueTypes::word_t: return sizeof(int16_t);
    case ValueTypes::dword_t: return sizeof(int32_t);
    case ValueTypes::qword_t: return sizeof(int64_t);
    case ValueTypes::float_t: return sizeof(float);
    case ValueTypes::double_t: return sizeof(double);
    default: throw unsupported_type_exception();
  }
}
ValueType ask_for_value_type() {
  for (size_t i = 0; i < VALUE_TYPES.size(); ++i) {
    cout << i + 1 << " - " << VALUE_TYPES[i].second;
    cout << "(" << value_type_size(VALUE_TYPES[i].first) << ")";
    cout << endl;
  }

  size_t choice = ask_for<size_t>("value type", "invalid choice", [](size_t n) {
      return n >= 1 && n <= VALUE_TYPES.size();
      });
  assert(choice >= 1 && choice <= VALUE_TYPES.size());

  return VALUE_TYPES[choice - 1].first;
}

vector<unsigned char> ask_for_value(ValueType type) {
  switch (type) {
    case ValueTypes::byte_t: return ask_value_for<int8_t>("byte", "invalid byte");
    case ValueTypes::word_t: return ask_value_for<int16_t>("word", "invalid word");
    case ValueTypes::dword_t: return ask_value_for<int32_t>("dword", "invalid dword");
    case ValueTypes::qword_t: return ask_value_for<int64_t>("qword", "invalid qword");
    case ValueTypes::float_t: return ask_value_for<float>("float", "invalid float");
    case ValueTypes::double_t: return ask_value_for<double>("double", "invalid double");
    default: throw unsupported_type_exception();
  }
}

vector<pair<ValueType, string>> make_value_types() {
  vector<pair<ValueType, string>> valueTypes = {
    make_pair(ValueTypes::byte_t, "byte"),
    make_pair(ValueTypes::word_t, "word"),
    make_pair(ValueTypes::dword_t, "dword"),
    make_pair(ValueTypes::qword_t, "qword"),
    make_pair(ValueTypes::float_t, "float"),
    make_pair(ValueTypes::double_t, "double")
  };

  assert(valueTypes.size() == VALUE_TYPE_COUNT);

  return valueTypes;
}

template <class T>
vector<unsigned char> ask_value_for(const string& valueName, const string& error) {
  stringstream ss;
  ss << valueName << " value";
  T value = ask_for<T>(ss.str(), error);
  unsigned char* bytes = reinterpret_cast<unsigned char*>(&value);
  return vector<unsigned char>(bytes, bytes + sizeof(T));
}
