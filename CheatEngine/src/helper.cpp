#include "helper.h"

#include <sstream>
#include <cctype>

bool ask_yes_no(const std::string& message) {
  std::stringstream query;
  query << message << " (y/n)";
  const auto choice = ask_for<char>(query.str(), "Invalid choice.",
                                    [] (char c) {
    c = tolower(c);
    return c == 'y' || c == 'n';
  });

  return tolower(choice) == 'y';
}
