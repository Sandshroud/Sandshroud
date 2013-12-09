
#pragma once

#define PLATFORM_WIN 1
#define PLATFORM_OTHER 2

#ifdef _WIN32
#define PLATFORM PLATFORM_WIN
#else
#define PLATFORM PLATFORM_OTHER
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <errno.h>
#include <string>
#include <vector>

#if PLATFORM == PLATFORM_WIN

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#else

#include <string.h>
#define MAX_PATH 1024

#endif
