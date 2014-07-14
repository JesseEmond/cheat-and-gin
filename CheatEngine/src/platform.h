#ifndef PLATFORM_H
#define PLATFORM_H

// required typedefs:
/*
- process id
- process handle
*/

#ifdef _WIN32
  #include <Windows.h>

	typedef DWORD pid_t;
  typedef HANDLE phandle_t;
#endif

#endif