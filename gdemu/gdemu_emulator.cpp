/**
 * EmulatorClass
 * $Id$
 * \file gdemu_emulator.cpp
 * \brief EmulatorClass
 * \date 2011-05-29 19:54GMT
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

// #include <...>
#include "gdemu_emulator.h"

// System includes
#include <istream>
#include <ostream>
#include <iostream>
#ifdef GDEMU_SDL
#include <SDL.h>
#include <SDL_thread.h>
#endif

// Project includes
#include "WProgram.h"
#include "GD.h"
#include "gdemu_system.h"
#include "gdemu_graphics_driver.h"
#include "gdemu_graphics_machine.h"
#include "gdemu_j1.h"
#include "gdemu_audio_driver.h"
#include "gdemu_audio_machine.h"
#include "gdemu_gameduino_spi.h"
#include "gdemu_keyboard.h"
#include "omp.h"

// using namespace ...;

namespace GDEMU {

EmulatorClass Emulator;

namespace {
	void (*s_Setup)() = NULL;
	void (*s_Loop)() = NULL;
	int s_Flags = 0;
	bool s_MasterRunning = false;

	int masterThread(void * = NULL)
	{
		System.makeMainThread();

		unsigned long taskId = 0;
		void *taskHandle;
		taskHandle = System.setThreadGamesCategory(&taskId);
		System.disableAutomaticPriorityBoost();
		System.makeRealtimePriorityThread();

		double targetSeconds = System.getSeconds();

		for (;;)
		{
			double deltaSeconds = GameduinoSPI.getRefreshTimeSeconds();

			//printf("main thread\n");
			System.makeRealtimePriorityThread();

			System.update();
			targetSeconds += deltaSeconds;

			J1.flagReset(GameduinoSPI.getJ1Reset() == 0x01);

			// Render lines, the j1 is run by the graphics machine inbetween lines to allow chasing them
			{
				GameduinoSPI.setVBlank(0);
				// System.delay(0);
				System.switchThread();

				unsigned long procStart = System.getMicros();
				GraphicsMachine.process();
				unsigned long procDelta = System.getMicros() - procStart;

				if (procDelta > 8000)
					printf("process: %i micros (%i ms)\r\n", (int)procDelta, (int)procDelta / 1000);
			}

			// Flip buffer and also give a slice of time to the duino main thread
			{
				GameduinoSPI.setVBlank(1);
				System.prioritizeDuinoThread();

#ifndef WIN32
				System.holdDuinoThread(); // vblank'd !
				System.resumeDuinoThread();
#endif

				unsigned long flipStart = System.getMicros();
				GraphicsMachine.flip();
				unsigned long flipDelta = System.getMicros() - flipStart;

				if (flipDelta > 8000)
					printf("flip: %i micros (%i ms)\r\n", (int)flipDelta, (int)flipDelta / 1000);

				//System.delay(2); // ensure slice of time to duino thread at cost of j1 cycles
				System.switchThread();
				System.unprioritizeDuinoThread();
			}

			//long currentMillis = millis();
			//long millisToWait = targetMillis - currentMillis;
			double currentSeconds = System.getSeconds();
			double secondsToWait = targetSeconds - currentSeconds;
			//if (millisToWait < -100) targetMillis = millis();
			if (secondsToWait < -0.25) // Skip freeze
			{
				//printf("skip freeze\n");
				targetSeconds = System.getSeconds();
			}

			//printf("millis to wait: %i", (int)millisToWait);

			if (secondsToWait > 0.0)
			{
				if (J1.isResetting())
				{
					System.delay((int)(secondsToWait * 1000.0));
				}
				else // run the j1 for the remaining time
				{
					System.makeNormalPriorityThread();

					J1.execute(0, (int)(secondsToWait * 1000000.0));
				}
			}

#ifdef WIN32
			System.holdDuinoThread(); // don't let the other thread hog cpu
			System.resumeDuinoThread();
#endif

			//fixRounding = -2 - fixRounding;
			// else delay(6);
		}
		System.revertThreadCategory(taskHandle);
		return 0;
	}

	int duinoThread(void * = NULL)
	{
		System.makeDuinoThread();

		unsigned long taskId = 0;
		void *taskHandle;
		taskHandle = System.setThreadGamesCategory(&taskId);
		System.disableAutomaticPriorityBoost();
		System.makeNormalPriorityThread();
		s_Setup();
		while (s_MasterRunning)
		{
			//printf("duino thread\n");
			s_Loop();
		}
		System.revertThreadCategory(taskHandle);
		return 0;
	}

	int audioThread(void * = NULL)
	{
		//printf("go sound thread");
		unsigned long taskId = 0;
		void *taskHandle;
		taskHandle = System.setThreadGamesCategory(&taskId);
		System.disableAutomaticPriorityBoost();
		System.makeHighPriorityThread();
		while (s_MasterRunning)
		{
			//printf("sound thread\n");
			if (s_Flags & EmulatorEnableAudio) AudioMachine.process();
			if (s_Flags & EmulatorEnableKeyboard) Keyboard.update();
			System.delay(10);
		}
		System.revertThreadCategory(taskHandle);
		return 0;
	}
}

void EmulatorClass::run(void (*setup)(), void (*loop)(), int flags)
{
	s_Setup = setup;
	s_Loop = loop;
	s_Flags = flags;

	System.begin();
	GameduinoSPI.begin();
	GraphicsDriver.begin();
	if (flags & EmulatorEnableAudio) AudioDriver.begin();
	if (flags & EmulatorEnableJ1) J1.begin();
	if (flags & EmulatorEnableKeyboard) Keyboard.begin();

	s_MasterRunning = true;

#ifdef GDEMU_SDL

	SDL_Thread *threadD = SDL_CreateThread(duinoThread, NULL);
	// TODO - Error handling

	SDL_Thread *threadA = SDL_CreateThread(audioThread, NULL);
	// TODO - Error handling

	masterThread();

	s_MasterRunning = false;
	SDL_WaitThread(threadD, NULL);
	SDL_WaitThread(threadA, NULL);

#else
	#pragma omp parallel num_threads(3)
	{
		// graphics
		#pragma omp master
		{
			masterThread();
			s_MasterRunning = false;
		}

		// arduino
		if (omp_get_thread_num() == 1)
		{
			duinoThread();
		}

		// sound
		if (omp_get_thread_num() == 2)
		{
			audioThread();
		}
	}
#endif /* #ifdef GDEMU_SDL */

	if (flags & EmulatorEnableKeyboard) Keyboard.end();
	if (flags & EmulatorEnableJ1) J1.end();
	if (flags & EmulatorEnableAudio) AudioDriver.end();
	GraphicsDriver.end();
	GameduinoSPI.end();
	System.end();
}

void EmulatorClass::setJ1RasterChasingCycles(int insnLimit)
{
	g_J1RasterChasingCycles = insnLimit;
}

} /* namespace GDEMU */

/* end of file */
