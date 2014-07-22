#ifndef CHEAT_ENGINE_H
#define CHEAT_ENGINE_H

#include "platform.h"

#include <string>
#include <unordered_map>

class CheatEngine
{
public:
	typedef unsigned char value_size_t;
	typedef unsigned char* value_t;

	typedef unsigned char* address_t;
	typedef std::unordered_multimap<address_t, address_t> addresses_t;

	const addresses_t& getMatchingAddresses() const { return m_addresses; }

  CheatEngine(pid_t process);
	~CheatEngine();

protected:
	addresses_t m_addresses;
  phandle_t m_process;



// Platform-specific:
public:
  static std::vector<pid_t> getProcessesWithName(const std::string& name);

  void addAddressesWithValue(const value_t& value, value_size_t size);
	void keepAddressesWithValue(const value_t& value, value_size_t size);
	void modifyMatchingAddresses(const value_t& value, value_size_t size) const;

private:
  phandle_t openProcess(pid_t id) const;
	void closeProcess(phandle_t handle) const;
};

#endif