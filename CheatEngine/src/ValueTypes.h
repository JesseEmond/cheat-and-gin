#ifndef VALUE_TYPES_H
#define VALUE_TYPES_H

#include "helper.h"
#include "ValueType.h"

#include <cstdint>
#include <sstream>
#include <memory>
#include <array>
#include <string>



const ValueType& ask_for_value_type();



template <class T>
struct SimpleValueType : public ValueType {
  SimpleValueType(const std::string& name) : ValueType(name) {}

  memory_t ask_value() const override;

  virtual memory_t memory_representation(const T& value) const;
  virtual bool is_valid(const T& value) const { return true; }
};



// since uint8_t is a char but the user is expecting to enter a number, we need
// to ask for a regular int and cast
struct UnsignedByte : SimpleValueType<std::uint32_t> {
  UnsignedByte() : SimpleValueType{"unsigned byte"} {}

  bool is_valid(const std::uint32_t& value) const override {
    return 0 <= value && value <= 255;  // check that it is 8 bits
  }
  memory_t memory_representation(const int32_t& value) const {
    uint8_t byte = value;
    return { byte }; // only extract 1 byte
  }
};

struct Character : SimpleValueType<char> {
  Character() : SimpleValueType{"character"} {}
};

struct Int16 : SimpleValueType<std::int16_t> {
  Int16() : SimpleValueType{"16-bit integer"} {}
};

struct Int32 : SimpleValueType<std::int32_t> {
  Int32() : SimpleValueType{"32-bit integer"} {}
};

struct Int64 : SimpleValueType<std::int64_t> {
  Int64() : SimpleValueType{"64-bit integer"} {}
};

struct Float : SimpleValueType<float> {
  Float() : SimpleValueType{"float"} {}
};

struct Double : SimpleValueType<double> {
  Double() : SimpleValueType{"double"} {}
};

struct String : SimpleValueType<std::string> {
  String() : SimpleValueType{"string"} {}

  memory_t memory_representation(const std::string& value) const override {
    // we can't look at the direct string representation, we need to copy the
    // actual chars (we're not searching for the char*, we're looking for the
    // actual chars!)
    return memory_t(std::begin(value), std::end(value));
  }
};


template <class T>
memory_t SimpleValueType<T>::ask_value() const {
  std::stringstream query, error;
  query << "Value for " << name;
  error << "Invalid " << name;
  auto value = ask_for<T>(query.str(), error.str(), [&](const T& value) {
    return is_valid(value);
  });
  return memory_representation(value);
}

template <class T>
memory_t SimpleValueType<T>::memory_representation(const T& value) const {
  auto bytes = reinterpret_cast<const memory_t::value_type*>(&value);
  return memory_t(bytes, bytes + sizeof(T));
}

#endif
