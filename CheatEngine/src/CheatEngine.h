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
	
	const addresses_t& getMatchingAddresses() const { return m_addresses; }
	void keepAddressesWithValue(const value_t& value, value_size_t size);

	void modifyMatchingAddresses(const value_t& value, value_size_t size) const;

	CheatEngine(const std::string& processName);
	~CheatEngine();

private:
	addresses_t m_addresses;
	process_id m_process;

	process_id openProcessWithName(const std::string& name) const;
	void closeProcess(process_id id) const;
};

#endif