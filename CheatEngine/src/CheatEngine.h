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

	CheatEngine(process_id process);
	~CheatEngine();

private:
	addresses_t m_addresses;
	process_id m_process;



// Platform-specific:
public:
	static std::vector<process_id> getProcessesWithName(const std::string& name);

	void keepAddressesWithValue(const value_t& value, value_size_t size);
	void modifyMatchingAddresses(const value_t& value, value_size_t size) const;

private:
	void openProcess(process_id id) const;
	void closeProcess(process_id id) const;
};

#endif