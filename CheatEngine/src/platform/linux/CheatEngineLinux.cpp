#include "../../CheatEngine.h"

#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

#include <iostream>
#include <string>
#include <tuple>
#include <fstream>
#include <sstream>

using namespace std;

// extracts the pid and name of the given process directory entry
// within /proc. If we fail to extract the information, throw
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
            } catch (invalid_argument) {} // could not read? ignore error
        }
        closedir(dir);
    } else {
        cerr << "Failed to open /proc to iterate over running processes." << endl;
    }
    return processes;
}

phandle_t CheatEngine::openProcess(pid_t id) const {
    stringstream memFilename;
    memFilename << "/proc/" << id << "/mem";

    phandle_t memoryFile = open(memFilename.str().c_str(), O_RDONLY);
    if (memoryFile < 0) {
        cerr << "Failed to open process memory file." << endl;
        exit(1);
    }
    if (ptrace(PTRACE_ATTACH, id, nullptr, nullptr) < 0) {
        cerr << "Failed to ptrace attach to process." << endl;
        exit(1);
    }
    if (waitpid(id, NULL, 0) < 0) {
        cerr << "Failed to waitpid on process." << endl;
        exit(1);
    }

    return memoryFile;
}

void CheatEngine::closeProcess(pid_t id, phandle_t handle) const {
    if (ptrace(PTRACE_DETACH, id, nullptr, nullptr) < 0) {
        cerr << "Failed to ptrace detach to process." << endl;
        exit(1);
    }
    if (close(handle) < 0) {
        cerr << "Failed to close process memory file." << endl;
        exit(1);
    }
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
