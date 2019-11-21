#pragma once

#include <stdio.h>

#ifdef _WIN32
	#include <windows.h>
#else
	#include <string.h>
#endif

#ifdef _WIN32
	#define strcasecmp _stricmp
	#define strncasecmp _strnicmp
#endif


