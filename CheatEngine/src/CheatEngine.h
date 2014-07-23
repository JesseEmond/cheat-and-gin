#ifndef CHEAT_ENGINE_H
#define CHEAT_ENGINE_H

#include "MemoryBlock.h"
#include "platform.h"

#include <string>
#include <list>
#include <vector>

class CheatEngine
{
public:
	typedef unsigned char value_size_t;
	typedef std::vector<unsigned char> value_t;

	typedef std::list<MemoryBlock> blocks_t;

	const blocks_t& getMatchingBlocks() const { return m_blocks; }

	CheatEngine(pid_t process);
	~CheatEngine();

protected:
	blocks_t m_blocks;
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