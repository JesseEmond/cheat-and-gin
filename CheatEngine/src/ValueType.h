#ifndef VALUE_TYPE_H
#define VALUE_TYPE_H

#include <vector>
#include <cstddef>

namespace ValueTypes {
	enum ValueTypes {
		byte_t = 0,
		word_t,
		dword_t,
		qword_t,
		float_t,
		double_t
	};
}
typedef ValueTypes::ValueTypes ValueType;

static const int VALUE_TYPE_COUNT = 6;

std::size_t value_type_size(ValueType type);
ValueType ask_for_value_type();
std::vector<unsigned char> ask_for_value(ValueType type);

#endif