#include <thread>
#include <chrono>
#include <iostream>

int main() {
  int target = 0x424242;

  for (int i = 0; i < 50; ++i) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // the cheat engine has to set this value to succeed.
    if (target == 999) {
      std::cout << "Integration test passed." << std::endl;
      return 0;
    }
  }

  std::cerr << "Integration test FAILED." << std::endl;
  return 1; // failed to cheat in time! The test failed.
}
