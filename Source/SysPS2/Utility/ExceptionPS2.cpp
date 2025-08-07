
#include "ExceptionPS2.h"
#include "Debug/DebugLog.h"
#include "Debug/DBGConsole.h"

void PS2Exception::Init()
{
    DebugLog::Log("PS2 Exception-Handler initialisiert.");
    CDebugConsole::Get()->Msg(0, "Exception-Handler aktiv.");
}

void PS2Exception::HandleException(const char* context, const char* message)
{
    DebugLog::Log("Exception in %s: %s", context, message);
    CDebugConsole::Get()->Msg(0, "Fehler in %s: %s", context, message);

}

// maybe not used