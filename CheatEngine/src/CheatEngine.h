#ifndef CHEAT_ENGINE_H
#define CHEAT_ENGINE_H

#include "platform.h"

#include <string>
#include <vector>

class CheatEngine
{
public:
	typedef char value_size_t;
	typedef char* value_t;

	typedef char* address_t;
	typedef std::vector<address_t> addresses_t;

	process_id findProcessWithName(const std::string& name) const;
	void close_process(process_id id) const;

	const addresses_t& getMatchingAddresses() const { return m_addresses; }
	void keepAddressesWithValue(const value_t& value, value_size_t size);

	void modifyMatchingAddresses(const value_t& value, value_size_t size);

	void reset() { m_addresses = addresses_t(); }

private:
	addresses_t m_addresses;
};

#endif