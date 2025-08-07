
#include "stdafx.h"
#include "Debug/DaedalusAssert.h"
#include "Debug/DBGConsole.h"
#include "DebugLog.h"

#include <cstdarg>
#include <cstdio>


extern bool g_bLog;
extern FILE* g_hOutputLog;


#ifdef DAEDALUS_ENABLE_ASSERTS

DaedalusAssertHook gAssertHook = NULL;

EAssertResult DaedalusAssert(const char *expression, const char *file, unsigned int line, const char *msg, ...)
{
    char buffer[1024];
    va_list va;
    va_start(va, msg);
    vsnprintf(buffer, sizeof(buffer), msg, va);
    va_end(va);

    char location[256];
    sprintf(location, "Location: %s(%u)", file, line);


    CDebugConsole* console = CDebugConsole::Get();
    console->Msg(0, "************************************************************");
    console->Msg(0, "Assert Failed:");
    console->Msg(0, expression);
    console->Msg(0, location);
    console->Msg(0, "");
    console->Msg(0, buffer);
    console->Msg(0, "************************************************************");

    return AR_IGNORE;
}

#endif // DAEDALUS_ENABLE_ASSERTS
