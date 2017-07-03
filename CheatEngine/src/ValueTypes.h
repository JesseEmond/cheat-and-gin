#ifndef VALUE_TYPES_H
#define VALUE_TYPES_H

#include "helper.h"
#include "ValueType.h"

#include <cstdint>
#include <sstream>
#include <memory>
#include <array>



const ValueType& ask_for_value_type();



template <class T>
struct SimpleValueType : public ValueType {
  SimpleValueType(const std::string& name) : ValueType(name) {}

  memory_t ask_value() const override;

protected:
  virtual T ask(const std::string& message, const std::string& error) const;
};



struct UnsignedByte : SimpleValueType<std::uint8_t> {
  UnsignedByte() : SimpleValueType{"unsigned byte"} {}

  std::uint8_t ask(const std::string& message,
                   const std::string& error) const override {
    // since uint8_t is a char but the user is expecting to enter a number, we
    // need to ask for a regular int and cast
    return ask_for<std::uint32_t>(message, error,
      [](uint32_t i) { return 0 <= i && i <= 255; });
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


template <class T>
memory_t SimpleValueType<T>::ask_value() const {
  std::stringstream query, error;
  query << "Value for " << name;
  error << "Invalid " << name;
  auto value = ask(query.str(), error.str());
  auto bytes = reinterpret_cast<memory_t::value_type*>(&value);
  return memory_t(bytes, bytes + sizeof(T));
}

template <class T>
T SimpleValueType<T>::ask(const std::string& message,
                          const std::string& error) const {
  return ask_for<T>(message, error);
}

#endif
