#include "CheatEngine.h"
#include "helper.h"

#include <iostream>
#include <vector>
#include <cassert>
using namespace std;

process_id ask_for_process();
process_id choose_process(const vector<process_id>& processes);



int main() {
	process_id process = ask_for_process();
	CheatEngine engine(process);

	//TODO
	cout << "todo" << endl;

	cin.get();
	return 0;
}



process_id ask_for_process() {
	vector<process_id> processes;

	do {
		string name = ask_for<string>("process name", "invalid process name");

		processes = CheatEngine::getProcessesWithName(name);

		if (processes.empty())
			cerr << "no process found for the given name." << endl;
	} while (processes.empty());
	assert(!processes.empty());

	process_id process = processes.front();

	if (processes.size() > 1)
		process = choose_process(processes);

	return process;
}

process_id choose_process(const vector<process_id>& processes) {
	assert(processes.size() > 1);

	cout << "Multiple possible processes, pick one of the following pids: ";
	copy(begin(processes), end(processes), ostream_iterator<process_id>(cout, "  "));
	cout << endl;

	return ask_for<process_id>("pid", "invalid process id", [&](process_id pid) {
		return find(begin(processes), end(processes), pid) != end(processes);
	});
}