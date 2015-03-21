#include "CheatEngine.h"

#include "helper.h"

#include <iostream>
#include <cassert>
#include <algorithm>
#include <iterator>

CheatEngine::CheatEngine(pid_t process)
        : m_processId(process) {
	m_process = openProcess(process);
}

CheatEngine::~CheatEngine() {
	closeProcess(m_processId, m_process);
}
