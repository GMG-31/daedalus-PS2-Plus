/*
Copyright (C) 2001 StrmnNrmn

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include "stdafx.h"
#include <stdio.h>
#include <stdarg.h>

#include "DebugLog.h"
#include "Debug/Dump.h"
#include "Debug/DBGConsole.h"

#include "Utility/IO.h"

#ifdef DAEDALUS_LOG

//*****************************************************************************
//
//*****************************************************************************
bool			g_bLog = false;
FILE *		g_hOutputLog	= NULL;

//*****************************************************************************
//
//*****************************************************************************
bool Debug_InitLogging()
{
	IO::Filename log_filename;

	Dump_GetDumpDirectory(log_filename, "");

	IO::Path::Append(log_filename, "daedalus.txt");

	//g_hOutputLog = fopen((const char*)log_filename, "w");
    FILE* g_hOutputLog = fopen("mass:/Dumps/daedalus.txt", "w");
    if (!g_hOutputLog) {
    CDebugConsole::Get()->Msg(0, "Fehler: konnte Log-Datei '%s' nicht öffnen!", log_filename);
}
	//CDebugConsole::Get()->Msg( 0, "fopen now", log_filename );
    
	return g_hOutputLog != nullptr;
}

//*****************************************************************************
//
//*****************************************************************************
void Debug_FinishLogging()
{
	if( g_hOutputLog )
	{
		fclose( g_hOutputLog );
		g_hOutputLog = NULL;
	}
}

//*****************************************************************************
//
//*****************************************************************************
void Debug_Print( const char * format, ... )
{
	if(g_bLog && format != NULL )
	{
		char buffer[1024+1];
		char * p = buffer;
		va_list va;
		// Parse the buffer:
		// Format the output
		va_start(va, format);
		// Don't use wvsprintf as it doesn't handle floats!
		//vsprintf(p, format, va);
		vsnprintf(p, sizeof(buffer), format, va);
        va_end(va);

		fprintf( g_hOutputLog, "%s\n", p );
      //  fflush(g_hOutputLog); 
	}
}


void Debug_Print_Str(const char* str)
{
    if (g_bLog && str != nullptr)
    {
        if (!g_hOutputLog)
        {
            Debug_InitLogging();
        }

        if (g_hOutputLog != nullptr)
        {
            fprintf(g_hOutputLog, "%s\n", str);
			fflush(g_hOutputLog); 
        }
    }
}

//*****************************************************************************
//
//*****************************************************************************
bool		Debug_GetLoggingEnabled()
{
	return g_bLog && (g_hOutputLog != NULL);
}

//*****************************************************************************
//
//*****************************************************************************
void		Debug_SetLoggingEnabled( bool enabled )
{
	g_bLog = enabled;
}


#endif // DAEDALUS_LOG
