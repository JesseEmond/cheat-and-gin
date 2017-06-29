#include "../../src/platform/linux/LinuxOS.h"

#include "catch.hpp"
using Catch::Equals;

#include <map>
#include <vector>

TEST_CASE("OSLinux can get processes by name", "[linux]") {
  struct LinuxOSTest : public LinuxOS {
    std::vector<pid_t> getRunningProcesses() const {
      return { 123, 234, 345, 456, 567 };
    }
    std::string getProcessName(pid_t pid) const {
      const std::map<pid_t, std::string> names = {
        {123, "test"}, {234, "test"}, {345, "program"},
        {456, "game"}, {567, "process"}
      };
      return names.at(pid);
    }
  } os;

  typedef std::vector<pid_t> pids;

  SECTION("can get a unique process by its name") {
    REQUIRE_THAT(os.getProcesses("game"), Equals(pids{456}));
  }
  SECTION("can get multiple processes sharing the same name") {
    REQUIRE_THAT(os.getProcesses("test"), Equals(pids{123, 234}));
  }
  SECTION("can not get processes with a wrong name") {
    REQUIRE_THAT(os.getProcesses("none"), Equals(pids{}));
  }
}
