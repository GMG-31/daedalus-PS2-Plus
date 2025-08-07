#include "stdafx.h"
#include "Debug/DBGConsole.h"
#include "DebugLog.h"

#ifdef DAEDALUS_DEBUG_CONSOLE

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <gsKit.h>

#include "Debug/DebugConsoleImpl.h"
#include "Test/BatchTest.h"

extern GSGLOBAL* gsGlobal;
extern GSFONTM* gsFontM;  // global bereitgestellt von deinem System

#define MAX_LINES 32
#define MAX_LINE_LENGTH 128

class IDebugConsole : public CDebugConsole
{
public:
    virtual void Msg(u32 type, const char* format, ...);
    virtual void MsgOverwriteStart();
    virtual void MsgOverwrite(u32 type, const char* format, ...);
    virtual void MsgOverwriteEnd();
    virtual void ForceRedraw() { RedrawConsole(); }


private:
    void RedrawConsole();
    char mLines[MAX_LINES][MAX_LINE_LENGTH];
    int mCurrentLine = 0;
};

template<> bool CSingleton<CDebugConsole>::Create()
{
    DAEDALUS_ASSERT_Q(mpInstance == NULL);
    mpInstance = new IDebugConsole();
    return true;
}

CDebugConsole::~CDebugConsole() {}

void IDebugConsole::RedrawConsole()
{
    if (!gsFontM || !gsGlobal)
        return;

    float consoleWidth = 320.0f;
    float consoleHeight = 192.0f;
    float consoleX = 640.0f - consoleWidth;
    float consoleY = 0.0f;

    // Hintergrund
    gsKit_prim_sprite(
        gsGlobal,
        consoleX, consoleY,
        consoleX + consoleWidth, consoleY + consoleHeight,
        0,
        GS_SETREG_RGBAQ(0x00, 0x00, 0x00, 0x60, 0x00)
    );

    float y = consoleY + 8.0f;
    for (int i = 0; i < MAX_LINES; ++i)
    {
        int idx = (mCurrentLine + 1 + i) % MAX_LINES;
        if (mLines[idx][0] != '\0')
        {
gsKit_fontm_print_scaled(
    gsGlobal,
    gsFontM,
    consoleX + 8.0f,
    y,
    1,
    0.3f, // oder 0.4f für noch kleiner
    GS_SETREG_RGBAQ(0xFF, 0xFF, 0xFF, 0x80, 0x00),
    mLines[idx]
);
            y += 16.0f;
            if (y > consoleY + consoleHeight - 16.0f)
                break;
        }
    }

    gsKit_queue_exec(gsGlobal);
    gsKit_sync_flip(gsGlobal);
    gsKit_vsync_wait();
}

void IDebugConsole::Msg(u32 type, const char* format, ...)
{
    va_list va;
    va_start(va, format);
    vsnprintf(mLines[mCurrentLine], MAX_LINE_LENGTH, format, va);
    va_end(va);
    
    Debug_Print_Str(mLines[mCurrentLine]);

    mCurrentLine = (mCurrentLine + 1) % MAX_LINES;
    RedrawConsole();
}

void IDebugConsole::MsgOverwriteStart() {}
void IDebugConsole::MsgOverwrite(u32 type, const char* format, ...)
{
    va_list va;
    va_start(va, format);
    vsnprintf(mLines[mCurrentLine], MAX_LINE_LENGTH, format, va);
    va_end(va);

    RedrawConsole();
}
void IDebugConsole::MsgOverwriteEnd() {}

#endif // DAEDALUS_DEBUG_CONSOLE


extern "C" void RedrawDebugConsole()
{
    CDebugConsole* console = CDebugConsole::Get();
    if (console)
        ((IDebugConsole*)console)->ForceRedraw();
}

