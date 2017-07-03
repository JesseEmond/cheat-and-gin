#include "CheatEngine.h"
#include "helper.h"
#include "ValueTypes.h"
#include "OS.h"

#include <iostream>
#include <vector>
#include <cassert>
#include <iterator>
#include <algorithm>
using namespace std;

pid_t ask_for_process(const OS& os);
pid_t choose_process(const vector<pid_t>& processes);
void pause();

int main() {
  auto os = OS::getOS();
  const auto pid = ask_for_process(*os);
  auto process = os->open(pid);
  CheatEngine engine{*process};

  cout << "What is the type of the searched value?" << endl;
  const auto& type = ask_for_value_type();
  auto value = type.ask_value();
  auto matches = engine.search(value);

  bool done = matches.totalMatches() == 0;

  while (!done) {
    cout << endl
         << matches.totalMatches() << " address(es) containing the value."
         << endl;
    done = !ask_for<bool>("Keep searching?", "invalid boolean");

    if (!done) {
      value = type.ask_value();
      matches = engine.narrowDown(matches, value);
    }
  }

  if (matches.totalMatches() > 0) {
    cout << "What value should the new address(es) have?" << endl;
    value = type.ask_value();

    engine.modify(matches, value);
    cout << "Value(s) modified." << endl;
  } else {
    cout << "No address(es) fit the given value(s)." << endl;
  }

  pause();
  return 0;
}



pid_t ask_for_process(const OS& os) {
  vector<pid_t> processes;

  do {
    string name = ask_for<string>("Process name", "invalid process name");

    processes = os.getProcesses(name);

    if (processes.empty())
      cerr << "No process found for the given name." << endl;
  } while (processes.empty());

  assert(!processes.empty());
  return processes.size() > 1 ? choose_process(processes) : processes.front();
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

void pause() {
  cin.ignore(cin.rdbuf()->in_avail());
  cin.get();
}
