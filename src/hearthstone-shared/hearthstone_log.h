/***
 * Demonstrike Core
 */

#pragma once

#include "Common.h"
#include "Singleton.h"
#include <consolelog/log.h>

class SERVER_DECL hLog : public basicLog, public Singleton<hLog>
{
public:
    basicLog* getBasic() { return this; }
    bool isOutProcess() { return false; };
    bool isOutDevelopment() { return false; };

private:
    virtual time_t GetTime() { return UNIXTIME; };

    virtual void AcquireLock() { logLock.Acquire(); };
    virtual void ReleaseLock() { logLock.Release(); };
    Mutex logLock;
};

#define sLog hLog::getSingleton()
