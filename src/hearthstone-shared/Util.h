/***
 * Demonstrike Core
 */

#pragma once

#include "Common.h"

extern SERVER_DECL time_t UNIXTIME;     /* update this every loop to avoid the time() syscall! */
extern SERVER_DECL tm g_localTime;

///////////////////////////////////////////////////////////////////////////////
// String Functions ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
std::vector<std::string> StrSplit(const std::string &src, const std::string &sep);

time_t convTimePeriod ( uint32 dLength, char dType);
HEARTHSTONE_INLINE uint32 secsToTimeBitFields(time_t secs)
{
    tm* lt = localtime(&secs);
    return (lt->tm_year - 100) << 24 | lt->tm_mon  << 20 | (lt->tm_mday - 1) << 14 | lt->tm_wday << 11 | lt->tm_hour << 6 | lt->tm_min;
}
