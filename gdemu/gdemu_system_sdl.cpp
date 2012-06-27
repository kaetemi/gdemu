/**
 * SystemSdlClass
 * $Id$
 * \file gdemu_system_sdl.cpp
 * \brief SystemSdlClass
 * \date 2012-06-27 11:28GMT
 * \author Jan Boon (Kaetemi)
 */

/*
 * Copyright (C) 2011-2012  Jan Boon (Kaetemi)
 *
 * This file is part of GAMEDUINO EMULATOR.
 * GAMEDUINO EMULATOR is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 2 of the
 * License, or (at your option) any later version.
 *
 * GAMEDUINO EMULATOR is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GAMEDUINO EMULATOR; see the file COPYING.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifdef GDEMU_SDL

// #include <...>
#include "gdemu_system_sdl.h"
#include "gdemu_system.h"

// Libraries
/*#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "avrt.lib")
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "msimg32.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")*/

// System includes
#ifdef WIN32
#else
#	include <sched.h>
#endif
#include <SDL_thread.h>

// Project includes

using namespace std;

namespace GDEMU {


SystemClass System;
SystemSdlClass SystemSdl;


#ifdef WIN32
//static LARGE_INTEGER s_PerformanceFrequency = { 0 };
//static LARGE_INTEGER s_PerformanceCounterBegin = { 0 };

//static HANDLE s_J1Thread = NULL;
//static HANDLE s_DuinoThread = NULL;
//static HANDLE s_MainThread = NULL;
#endif

static int s_DuinoThread = 0;
static int s_MainThread = 0;

//static CRITICAL_SECTION s_CriticalSection;


void SystemClass::_begin()
{
	//QueryPerformanceFrequency(&s_PerformanceFrequency);
	//QueryPerformanceCounter(&s_PerformanceCounterBegin);
	//InitializeCriticalSection(&s_CriticalSection);
	SDL_Init(0);
}

void SystemClass::_update()
{

}

void SystemClass::_end()
{
	SDL_Quit();
	//DeleteCriticalSection(&s_CriticalSection);
}
/*
void SystemClass::enterCriticalSection()
{
	//EnterCriticalSection(&s_CriticalSection);
}

void SystemClass::leaveCriticalSection()
{
	//LeaveCriticalSection(&s_CriticalSection);
}
*/
void SystemClass::disableAutomaticPriorityBoost()
{
#ifdef WIN32
	SetThreadPriorityBoost(GetCurrentThread(), TRUE);
#endif
}
void SystemClass::makeLowPriorityThread()
{
#ifdef WIN32
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_LOWEST);
#endif
}
void SystemClass::makeNormalPriorityThread()
{
#ifdef WIN32
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
#endif
}

void SystemClass::makeHighPriorityThread()
{
#ifdef WIN32
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
#endif
}

void SystemClass::makeHighestPriorityThread()
{
#ifdef WIN32
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
#endif
}

void SystemClass::makeRealtimePriorityThread()
{
#ifdef WIN32
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
#endif
}

void SystemClass::makeMainThread()
{
#ifdef WIN32
	if (!DuplicateHandle(
		GetCurrentProcess(),
		GetCurrentThread(),
		GetCurrentProcess(),
		&s_MainThread,
		0,
		TRUE,
		DUPLICATE_SAME_ACCESS))
		SystemSdl.ErrorWin32();
#endif
	s_MainThread = SDL_ThreadID();
}

bool SystemClass::isMainThread()
{
#ifdef WIN32
	HANDLE currentThread;
	if (!DuplicateHandle(
		GetCurrentProcess(),
		GetCurrentThread(),
		GetCurrentProcess(),
		&currentThread,
		0,
		TRUE,
		DUPLICATE_SAME_ACCESS))
		SystemSdl.ErrorWin32();
	return currentThread == s_MainThread;
#endif
	return SDL_ThreadID() == s_MainThread;
}

// Duino thread control
void SystemClass::makeDuinoThread()
{
#ifdef WIN32
	if (!DuplicateHandle(
		GetCurrentProcess(),
		GetCurrentThread(),
		GetCurrentProcess(),
		&s_DuinoThread,
		0,
		TRUE,
		DUPLICATE_SAME_ACCESS))
		SystemSdl.ErrorWin32();
#endif
	s_DuinoThread = SDL_ThreadID();
}

bool SystemClass::isDuinoThread()
{
#ifdef WIN32
	HANDLE currentThread;
	if (!DuplicateHandle(
		GetCurrentProcess(),
		GetCurrentThread(),
		GetCurrentProcess(),
		&currentThread,
		0,
		TRUE,
		DUPLICATE_SAME_ACCESS))
		SystemSdl.ErrorWin32();
	return currentThread == s_DuinoThread;
#endif
	return SDL_ThreadID() == s_DuinoThread;
}

void SystemClass::prioritizeDuinoThread()
{
#ifdef WIN32
	if (s_DuinoThread != NULL)
		SetThreadPriority(s_DuinoThread, THREAD_PRIORITY_HIGHEST);
#endif
}

void SystemClass::unprioritizeDuinoThread()
{
#ifdef WIN32
	if (s_DuinoThread != NULL)
		SetThreadPriority(s_DuinoThread, THREAD_PRIORITY_NORMAL);
#endif
}

void SystemClass::holdDuinoThread()
{
#ifdef WIN32
	if (0 > SuspendThread(s_DuinoThread))
		SystemSdl.Error(TEXT("SuspendThread  FAILED"));
#endif
// TODO - Important
}

void SystemClass::resumeDuinoThread()
{
#ifdef WIN32
	if (0 > ResumeThread(s_DuinoThread))
		SystemSdl.Error(TEXT("ResumeThread  FAILED"));
#endif
// TODO - Important
}



/*
void SystemClass::prioritizeJ1Thread()
{
	if (s_J1Thread != NULL)
		SetThreadPriority(s_J1Thread, THREAD_PRIORITY_HIGHEST);
}

void SystemClass::unprioritizeJ1Thread()
{
	if (s_J1Thread != NULL)
		SetThreadPriority(s_J1Thread, THREAD_PRIORITY_NORMAL);
}

void SystemClass::makeJ1Thread()
{
	s_J1Thread = GetCurrentThread();
}
*/

void *SystemClass::setThreadGamesCategory(unsigned long *refId)
{
#ifdef WIN32
	HANDLE h = AvSetMmThreadCharacteristics(TEXT("Games"), refId);
	if (!h) SystemSdl.ErrorWin32();
	return h;
#endif
}

void SystemClass::revertThreadCategory(void *taskHandle)
{
#ifdef WIN32
	AvRevertMmThreadCharacteristics(taskHandle);
#endif
}

void SystemClass::switchThread()
{
#ifdef WIN32
	SwitchToThread();
#else
	sched_yield();
#endif
}

double SystemClass::getSeconds()
{
	return (double)SDL_GetTicks() / 1000.0;
}

long SystemClass::getMillis()
{
	return SDL_GetTicks();
}

long SystemClass::getMicros()
{
	return SDL_GetTicks() * 1000; // FIXME - Higher Resolution
}

long SystemClass::getFreqTick(int hz)
{
	return (long)SDL_GetTicks() * (long)hz / 1000; // FIXME - Higher Resolution
}

void SystemClass::delay(int ms)
{
	SDL_Delay(ms);
}

void SystemClass::delayMicros(int us)
{
	long endMicros = getMicros() + (long)us;;
	do
	{
		switchThread();
	} while (getMicros() < endMicros);
	//Sleep(us / 1000);
}


void SystemSdlClass::Error(char *message)
{
	printf("Error: %s", message);
	exit(1);
}

void SystemSdlClass::ErrorSdl()
{
	printf("ErrorSdl: %s", SDL_GetError());
	exit(1);
}

/*
tstring SystemSdlClass::GetWin32ErrorString(DWORD dwError)
{
	// convert win32 error number to string

	LPTSTR lpMsgBuf;

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dwError,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

	tstring result = tstring(lpMsgBuf);

    LocalFree(lpMsgBuf);

	return result;
}

tstring SystemSdlClass::GetWin32LastErrorString()
{
	// put the last win32 error in a string and add the error number too
	DWORD dwError = GetLastError();
	tstringstream buffer;
	buffer << GetWin32ErrorString(dwError)
		<< TEXT(" (error: ") << dwError << ")";
	return buffer.str();
}

void SystemSdlClass::Error(const tstring &message)
{
	// exit with message
	MessageBox(NULL, (LPCTSTR)message.c_str(), TEXT("Error"), MB_OK | MB_ICONERROR);
	tcout << TEXT("Error: ") << message << endl;
	exit(EXIT_FAILURE);
}

void SystemSdlClass::Warning(const tstring &message)
{
	// show a warning box and send to output
	MessageBox(NULL, (LPCTSTR)message.c_str(), TEXT("Warning"), MB_OK | MB_ICONWARNING);
	tcout << TEXT("Warning: ") << message << endl;
}

void SystemSdlClass::Debug(const tstring &message)
{
	// send a debug to output
	tcout << TEXT("Debug: ") << message << endl;
}

void SystemSdlClass::ErrorWin32()
{
	// crash with last win32 error string
	Error(GetWin32LastErrorString());
}

void SystemSdlClass::ErrorHResult(HRESULT hr)
{
	Error(TEXT("ErrorHResult")); // fixme :p
}

#ifdef _UNICODE
tstring SystemSdlClass::ToTString(const std::wstring &s) { return s; }
tstring SystemSdlClass::ToTString(const std::string &s)
#else
tstring SystemSdlClass::ToTString(const std::string &s) { return s; }
tstring SystemSdlClass::ToTString(const std::wstring &s)
#endif
{
	tstring result(s.length(), 0);
	// copy from one to another
	std::copy(s.begin(), s.end(), result.begin());
	return result;
}

#ifdef _UNICODE
std::wstring SystemSdlClass::ToWString(const tstring &s) { return s; }
std::string SystemSdlClass::ToAString(const tstring &s)
#else
std::string SystemSdlClass::ToAString(const tstring &s) { return s; }
std::wstring SystemSdlClass::ToWString(const tstring &s)
#endif
{
#ifdef _UNICODE
	string
#else
	wstring
#endif
		result(s.length(), 0);
	// copy from one to another
	std::copy(s.begin(), s.end(), result.begin());
	return result;
}
*/

} /* namespace GDEMU */

#endif /* #ifdef GDEMU_SDL */

/* end of file */
