#include "../../src/OS.h"
#include "../../src/CheatEngine.h"
#include "../../src/ValueTypes.h"

#include <sys/wait.h>
#include <unistd.h>
#include <cassert>
#include <iostream>
#include <thread>
#include <chrono>

void launch_target();
bool cheat(pid_t target);

int main() {
  pid_t pid = fork();
  assert(pid >= 0);

  if (pid == 0) {
    launch_target();
  } else {
    // give some time for the process to be ready.
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    assert(cheat(pid));
  }

  return 0;
}

void launch_target() {
  execl("./CheatEngine/integration/bin/IntegrationTarget", 0, 0);
  assert(false);  // problem with execl
}

bool cheat(pid_t target) {
  std::cout << "Target pid: " << target << std::endl;
  auto os = OS::getOS();
  auto process = os->open(target);
  CheatEngine engine{*process};

  const Int32 type;
  const auto value = type.representation(0x424242);
  const auto matches = engine.search(value);
  assert(matches.any());

  engine.modify(matches, type.representation(999));

  int status;
  while (waitpid(target, &status, 0) == -1)
    ;
  std::cout << "Target status: " << status << std::endl;

  return status == 0;
}
