#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef _WIN32
  #include <Windows.h>
	typedef DWORD process_id;
#endif

#endif