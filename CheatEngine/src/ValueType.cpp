#include "ValueType.h"

#include "helper.h"

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <cassert>
using namespace std;

// Declarations
std::vector<std::pair<ValueType, std::string>> make_value_types();


// Globals
const auto VALUE_TYPES = make_value_types();


// Definitions
ValueType ask_for_value_type() {
	for (int i = 0; i < VALUE_TYPES.size(); ++i) {
		cout << i + 1 << " - " << VALUE_TYPES[i].second;
		cout << "(" << static_cast<int>(VALUE_TYPES[i].first) << ")";
		cout << endl;
	}

	int choice = ask_for<int>("value size", "invalid choice", [](int n) {
		return n >= 1 && n <= VALUE_TYPE_COUNT;
	});
	assert(choice >= 1 && choice <= VALUE_TYPE_COUNT);

	return VALUE_TYPES[choice - 1].first;
}

std::vector<std::pair<ValueType, std::string>> make_value_types() {
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