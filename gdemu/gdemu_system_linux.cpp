/**
 * SystemLinuxClass
 * $Id$
 * \file gdemu_system_linux.cpp
 * \brief SystemLinuxClass
 * \date 2012-06-29 14:50GMT
 * \author Jan Boon (Kaetemi)
 */

/*
 * Copyright (C) 2011  Jan Boon (Kaetemi)
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

#ifndef WIN32

// #include <...>
#include "gdemu_system_linux.h"
#include "gdemu_system.h"

// System includes
#include <sys/types.h>
#include <signal.h>

// Project includes

using namespace std;

namespace GDEMU {

SystemClass System;
SystemLinuxClass SystemLinux;

//static pthread_t s_J1Thread = 0;
static pthread_t s_DuinoThread = 0;
static pthread_t s_MainThread = 0;

static sigset_t s_DuinoSigSet;
pthread_mutex_t s_DuinoSuspendMutex = PTHREAD_MUTEX_INITIALIZER;

void SystemClass::_begin()
{
#ifdef GDEMU_SDL
	SDL_Init(0);
#endif
	sigemptyset(&s_DuinoSigSet);
	sigaddset(&s_DuinoSigSet, SIGUSR1);
}

void SystemClass::_update()
{

}

void SystemClass::_end()
{
#ifdef GDEMU_SDL
	SDL_Quit();
#endif
}

void SystemClass::disableAutomaticPriorityBoost()
{
	//SetThreadPriorityBoost(GetCurrentThread(), TRUE);
}
void SystemClass::makeLowPriorityThread()
{
	//SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_LOWEST);
}
void SystemClass::makeNormalPriorityThread()
{
	//SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
}

void SystemClass::makeHighPriorityThread()
{
	//SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
}

void SystemClass::makeHighestPriorityThread()
{
	//SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
}

void SystemClass::makeRealtimePriorityThread()
{
	//SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
}

void SystemClass::makeMainThread()
{
	s_MainThread = pthread_self();
	sigprocmask(SIG_BLOCK, &s_DuinoSigSet, NULL);
}

bool SystemClass::isMainThread()
{
	return pthread_self() == s_MainThread;
}

static void suspendDuinoWait(int signum)
{
	if (signum == SIGUSR1)
	{
		if (pthread_mutex_lock(&s_DuinoSuspendMutex))
			printf("Cannot lock");
		pthread_mutex_unlock(&s_DuinoSuspendMutex);
	}
	else
	{
		SystemLinux.Error("Bad duino suspend signal");
	}
}

// Duino thread control
void SystemClass::makeDuinoThread()
{
	s_DuinoThread = pthread_self();
	signal(SIGUSR1, suspendDuinoWait);
}

bool SystemClass::isDuinoThread()
{
	return pthread_self() == s_DuinoThread;
}

void SystemClass::prioritizeDuinoThread()
{
	//if (s_DuinoThread != NULL)
	//	SetThreadPriority(s_DuinoThread, THREAD_PRIORITY_HIGHEST);
}

void SystemClass::unprioritizeDuinoThread()
{
	//if (s_DuinoThread != NULL)
	//	SetThreadPriority(s_DuinoThread, THREAD_PRIORITY_NORMAL);
}

void SystemClass::holdDuinoThread()
{
	if (!pthread_mutex_trylock(&s_DuinoSuspendMutex))
	{
		if (pthread_kill(s_DuinoThread, SIGUSR1))
			SystemLinux.Error("Send user signal suspend fail");
	}
	else
	{
		printf("Trylock duino fail");
	}
}

void SystemClass::resumeDuinoThread()
{
	pthread_mutex_unlock(&s_DuinoSuspendMutex);
}

void *SystemClass::setThreadGamesCategory(unsigned long *refId)
{
	//HANDLE h = AvSetMmThreadCharacteristics(TEXT("Games"), refId);
	//if (!h) SystemLinux.ErrorWin32();
	//return h;
	return NULL;
}

void SystemClass::revertThreadCategory(void *taskHandle)
{
	//AvRevertMmThreadCharacteristics(taskHandle);
}

void SystemClass::switchThread()
{
	sched_yield();
}

double SystemClass::getSeconds()
{
	timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	return (double)t.tv_sec + (double)t.tv_nsec * 0.000000001;// / 1000000000.0;
}

long SystemClass::getMillis()
{
	timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	return t.tv_nsec / 1000000 + t.tv_sec * 1000;
}

long SystemClass::getMicros()
{
	timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	return (t.tv_nsec / 1000) + t.tv_sec * 1000000;
}

long SystemClass::getFreqTick(int hz)
{
	return getMicros() * (long)hz / 1000000; // FIXME - Higher Resolution
}

void SystemClass::delay(int ms)
{
	long endMillis = getMillis() + (long)ms;
	int sleepMillis = endMillis - getMillis();
	do
	{
		if (sleepMillis >= 1000) sleep(sleepMillis / 1000);
		else usleep(sleepMillis * 1000);
		sleepMillis = endMillis - getMillis();
	} while (sleepMillis > 0);
}

void SystemClass::delayMicros(int us)
{
	long endMicros = getMicros() + (long)us;
	int sleepMicros = (long)us;
	do
	{
		if (endMicros >= 1000) sleep(endMicros / 1000);
		else usleep(endMicros * 1000);
		sleepMicros = endMicros - getMicros();
	} while (sleepMicros > 0);

	/*if (us >= 1000000) sleep(us / 1000000);
	else usleep(us);*/

	/*

	long endMicros = getMicros() + (long)us;;
	do
	{
		switchThread();
	} while (getMicros() < endMicros);

	*/
}

void SystemLinuxClass::Error(char *message)
{
	printf("Error: %s", message);
	exit(1);
}

} /* namespace GDEMU */

#endif /* #ifndef WIN32 */

/* end of file */
