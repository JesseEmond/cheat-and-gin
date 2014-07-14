#ifndef VALUE_TYPE_H
#define VALUE_TYPE_H

namespace ValueTypes {
	enum ValueTypes {
		byte_t = 1,
		word_t = 2,
		dword_t = 4,
		qword_t = 8,
		float_t = 4,
		double_t = 8
	};
}
typedef ValueTypes::ValueTypes ValueType;

static const int VALUE_TYPE_COUNT = 6;

ValueType ask_for_value_type();

#endif