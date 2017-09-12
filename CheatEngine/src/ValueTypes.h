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

  memory_t askValue() const override;

  virtual memory_t representation(const T& value) const;
  virtual bool isValid(const T& value) const { return true; }
  virtual std::istream& read(std::istream& in, T& t) const { return in >> t; }
};



// since uint8_t is a char but the user is expecting to enter a number, we need
// to ask for a regular int and cast
struct UnsignedByte : SimpleValueType<std::uint32_t> {
  UnsignedByte() : SimpleValueType{"unsigned byte"} {}

  bool isValid(const std::uint32_t& value) const override {
    return 0 <= value && value <= 255;  // check that it is 8 bits
  }
  memory_t representation(const int32_t& value) const {
    uint8_t byte = value;
    return { static_cast<byte_t>(byte) }; // only extract 1 byte
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

  std::istream& read(std::istream& in, std::string& value) const override {
    in.ignore();
    return std::getline(in, value);
  }

  memory_t representation(const std::string& value) const override {
    // we can't look at the direct std::string representation, we need to copy
    // the actual chars (we're not searching for the internal char* and other
    // members, we're looking for the actual chars!)
    return memory_t(std::begin(value), std::end(value));
  }
};


template <class T>
memory_t SimpleValueType<T>::askValue() const {
  std::stringstream query, error;
  query << "Value for " << name;
  error << "Invalid " << name;
  const Validator<T> validate = [&](const T& value) { return isValid(value); };
  const Reader<T> read_value = [&](std::istream& in, T& t) -> std::istream& { return read(in, t); };
  const auto value = ask_for<T>(query.str(), error.str(), validate, read_value);
  return representation(value);
}

template <class T>
memory_t SimpleValueType<T>::representation(const T& value) const {
  auto bytes = reinterpret_cast<const memory_t::value_type*>(&value);
  return memory_t(bytes, bytes + sizeof(T));
}

#endif
