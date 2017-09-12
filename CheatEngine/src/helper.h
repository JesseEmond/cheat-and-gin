#ifndef HELPER_H
#define HELPER_H

#include <string>
#include <iostream>
#include <functional>

// Declarations
template <class T>
using Validator = std::function<bool(const T&)>;

template <class T>
using Reader = std::function<std::istream&(std::istream&, T&)>;

template <class T>
constexpr Validator<T> DefaultValidator() {
  return [] (const T&) { return true; };
}

template <class T>
constexpr Reader<T> DefaultReader() {
  return [] (std::istream& in, T& t) -> std::istream& { return in >> t; };
}

template <class T>
T ask_for(const std::string& message, const std::string& error,
          Validator<T> validate = DefaultValidator<T>(),
          Reader<T> read = DefaultReader<T>());

/**
 * Ask for a yes/no boolean answer.
 */
bool ask_yes_no(const std::string& message);



// Definitions
template <class T>
T ask_for(const std::string& message, const std::string& error,
          Validator<T> validate, Reader<T> read) {
  static const char PROMPT = ':';
  T t;
  std::cout << message << PROMPT << " ";
  while (!read(std::cin, t) || !validate(t)) {
    std::cerr << error << std::endl;
    std::cout << message << PROMPT << " ";

    if (!std::cin) {
      std::cin.clear();
      std::cin.ignore(std::cin.rdbuf()->in_avail());
    }
  }

  return t;
}

#endif
