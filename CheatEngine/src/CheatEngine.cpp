#include "CheatEngine.h"

CheatEngine::CheatEngine(const std::string& processName) {
	m_process = openProcessWithName(processName);
}

CheatEngine::~CheatEngine() {
	closeProcess(m_process);
}