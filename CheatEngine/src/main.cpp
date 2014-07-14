#include "CheatEngine.h"
#include "helper.h"
#include "ValueType.h"

#include <iostream>
#include <vector>
#include <cassert>
using namespace std;

pid_t ask_for_process();
pid_t choose_process(const vector<pid_t>& processes);


int main() {
  pid_t process = ask_for_process();
	CheatEngine engine(process);

  ValueType type = ask_for_value_type();
  vector<char> value = ask_for_value(type);

  // pause
	char c;
  cin >> c;

	return 0;
}



pid_t ask_for_process() {
	vector<pid_t> processes;

	do {
		string name = ask_for<string>("process name", "invalid process name");

		processes = CheatEngine::getProcessesWithName(name);

		if (processes.empty())
			cerr << "no process found for the given name." << endl;
	} while (processes.empty());
	assert(!processes.empty());

  pid_t process = processes.front();

	if (processes.size() > 1)
    process = choose_process(processes);

	return process;
}

pid_t choose_process(const vector<pid_t>& processes) {
	assert(processes.size() > 1);

	cout << "Multiple possible processes, pick one of the following pids: ";
  copy(begin(processes), end(processes), ostream_iterator<pid_t>(cout, "  "));
	cout << endl;

  return ask_for<pid_t>("pid", "invalid process id", [&](pid_t pid) {
		return find(begin(processes), end(processes), pid) != end(processes);
	});
}