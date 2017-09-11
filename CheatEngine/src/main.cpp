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
Matches search_addresses(const CheatEngine& engine, const ValueType& type);
void show_matches(const Matches& matches);
void modify_addresses(CheatEngine& engine, const ValueType& type,
                      const Matches& matches);


int main() {
  auto os = OS::getOS();
  const auto pid = ask_for_process(*os);
  auto process = os->open(pid);
  CheatEngine engine{*process};

  cout << "What is the type of the searched value?" << endl;
  const auto& type = ask_for_value_type();

  const auto matches = search_addresses(engine, type);

  if (matches.any()) {
    modify_addresses(engine, type, matches);
  }

  return 0;
}



pid_t ask_for_process(const OS& os) {
  vector<pid_t> processes;

  do {
    string name = ask_for<string>("Process name", "invalid process name");

    processes = os.getProcesses(name);

    if (processes.empty())
      cerr << "No process found with the given name." << endl;
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

Matches search_addresses(const CheatEngine& engine, const ValueType& type) {
  auto value = type.askValue();
  auto matches = engine.search(value);

  bool done;
  do {
    show_matches(matches);
    done = engine.doneSearching(matches) || !ask_yes_no("Keep searching?");

    if (!done) {
      value = type.askValue();
      matches = engine.narrowDown(matches, value);
    }
  } while(!done);

  return matches;
}

void show_matches(const Matches& matches) {
  cout << endl;

  if (matches.any()) cout << matches.totalMatches();
  else cout << "No";

  cout << " address(es) containing the value."
       << endl;
}

void modify_addresses(CheatEngine& engine, const ValueType& type,
                      const Matches& matches) {
  cout << "What value should the new address(es) have?" << endl;
  const auto value = type.askValue();

  engine.modify(matches, value);
  cout << "Value(s) modified." << endl;
}
