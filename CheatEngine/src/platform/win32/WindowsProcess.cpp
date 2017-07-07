#include "WindowsProcess.h"

#include <TlHelp32.h>
#include <Windows.h>

#include <iostream>


bool can_cheat_page(const MEMORY_BASIC_INFORMATION& page);


WindowsProcess::WindowsProcess(pid_t pid)
  : Process{pid},
    handle{OpenProcess(PROCESS_ALL_ACCESS, false, pid)} {
  if (!handle) {
    std::cerr << "Failed to open process. "
              << "Do you have the required rights?" << std::endl;
    exit(1);
  }
}

WindowsProcess::~WindowsProcess() {
  if (!CloseHandle(handle)) {
    // we're in a dtor, nothing else we can do except report and ignore
    std::cerr << "Failed to close process." << std::endl;
  }
}

std::vector<MemoryPage> WindowsProcess::getCheatablePages() const {
  std::vector<MemoryPage> pages;

  for (address_t address = nullptr, MEMORY_BASIC_INFORMATION page;
      VirtualQueryEx(handle, address, &page, sizeof(page)) == sizeof(page);
      address = static_cast<address_t>(page.BaseAddress) + page.RegionSize) {
    if (can_cheat_page(page)) {
      pages.emplace_back(address, page.RegionSize);
    }
  }

  return pages;
}

memory_t WindowsProcess::read(MemoryPage page) const {
  memory_t memory(page.size);

  DWORD total;
  if (!ReadProcessMemory(handle, page.address, memory.data(),
                         page.size, &total)) {
    std::cerr << "Failed to read process memory." << std::endl;
    exit(1);
  }

  memory.resize(total);
  return memory;
}

void WindowsProcess::write(address_t address, const memory_t& value) {
  if (!WriteProcessMemory(handle, address,
                          value.data(), value.size(), nullptr)) {
    std::cerr << "Failed to write to the process' memory." << std::endl;
  }
}


bool can_cheat_page(const MEMORY_BASIC_INFORMATION& page) {
  return page.State   == MEM_COMMIT  &&
         page.Type    == MEM_PRIVATE &&
         page.Protect == PAGE_READWRITE;
}
