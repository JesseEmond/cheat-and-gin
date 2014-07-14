#include "CheatEngine.h"

#include "helper.h"

#include <iostream>
#include <cassert>
#include <algorithm>
#include <iterator>

CheatEngine::CheatEngine(process_id process) {
	m_process = process;
	openProcess(m_process);
}

CheatEngine::~CheatEngine() {
	closeProcess(m_process);
}