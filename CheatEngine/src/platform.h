#ifndef PLATFORM_H
#define PLATFORM_H

/**
 * Required typedefs per platform:
 * - process id
 * - process handle
 */

#ifdef _WIN32
#include <Windows.h>

typedef DWORD pid_t;
typedef HANDLE phandle_t;
#endif

#ifdef LINUX
#include <sys/types.h> // defines pid_t
typedef int phandle_t; // file descriptor for the mem of the process
#endif

#endif
