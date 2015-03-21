#include "../../CheatEngine.h"

#include <dirent.h>

#include <iostream>
#include <string>
#include <tuple>
#include <fstream>
#include <sstream>

using namespace std;

// extracts the pid and name of the given process directory entry
// within /proc. If we fail to extract the information, throws
// an invalid_argument exception.
pair<pid_t, string> get_process_pid_name(const dirent* procEntry);


void CheatEngine::addAddressesWithValue(const value_t& value, value_size_t size) {
}

void CheatEngine::keepAddressesWithValue(const value_t& value, value_size_t size) {
}

void CheatEngine::modifyMatchingAddresses(const value_t& value, value_size_t size) const {
}

vector<pid_t> CheatEngine::getProcessesWithName(const std::string& name) {
    vector<pid_t> processes;
    auto dir = opendir("/proc");
    if (dir != nullptr) {
        dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            try {
                pid_t pid;
                string pname;

                tie(pid, pname) = get_process_pid_name(entry);
                if (name == pname) {
                    processes.push_back(pid);
                }

            } catch (invalid_argument) {}
        }
        closedir(dir);
    } else {
        cerr << "Failed to open /proc/ to iterate over running processes." << endl;
    }
    return processes;
}

phandle_t CheatEngine::openProcess(pid_t id) const {
    return phandle_t();
}

void CheatEngine::closeProcess(phandle_t handle) const {
}

pair<pid_t, string> get_process_pid_name(const dirent* procEntry) {
    string pidStr(procEntry->d_name);
    pid_t pid = stol(pidStr); // let it throw invalid_argument if fails

    stringstream procNameFilename;
    procNameFilename << "/proc/" << pid << "/comm";
    ifstream procNameFile(procNameFilename.str());

    if (!procNameFile) {
        cerr << "Failed to open comm file for pid " << pid << endl;
        throw invalid_argument("Faile to open comm file for process");
    }

    string name;
    procNameFile >> name;
    return make_pair(pid, name);
}
