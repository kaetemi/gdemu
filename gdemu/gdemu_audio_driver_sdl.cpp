/**
 * AudioDriverClass
 * $Id$
 * \file gdemu_audio_driver_sdl.cpp
 * \brief AudioDriverClass
 * \date 2012-06-27 11:45GMT
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
#include "gdemu_audio_driver.h"

// System includes
#include "gdemu_system_sdl.h"

// Project includes
#include "gdemu_audio_machine.h"

// using namespace ...;

namespace GDEMU {

AudioDriverClass AudioDriver;
/*
#define REFTIMES_PER_SEC  10000000
#define REFTIMES_PER_MILLISEC  10000

static IMMDeviceEnumerator *s_MMDeviceEnumerator = NULL;
static IMMDevice *s_MMDevice = NULL;
static IAudioClient *s_AudioClient = NULL;
static IAudioRenderClient *s_AudioRenderClient = NULL;

static unsigned int s_BufferFrameCount = 0;
static unsigned int s_NumFramesAvailable = 0;

static double s_TestRad;
*/

static int s_AudioFrequency = 0;

/*
#define D_GDEMU_AUDIOCHANNELS 2
#define D_GDEMU_AUDIOBITS 16*/

namespace {

void sdlAudioCallback(void *userdata, Uint8 *stream, int len)
{
	AudioMachine.process((short *)stream, len / 4); // 2 channels, 2 bytes per channel (hopefully...)
}

} /* anonymous namespace */

void AudioDriverClass::begin()
{
	SDL_InitSubSystem(SDL_INIT_AUDIO);

	SDL_AudioSpec desired;
	SDL_AudioSpec obtained;

	desired.freq = 22050;
	desired.format = AUDIO_S16LSB;
	desired.channels = 2;
	desired.samples = 512; // 2048; //8192;
	desired.callback = sdlAudioCallback;
	desired.userdata = NULL;

	if (SDL_OpenAudio(&desired, &obtained) < 0)
		SystemSdl.ErrorSdl();

	s_AudioFrequency = obtained.freq;

	SDL_PauseAudio(0);
}

bool AudioDriverClass::update()
{
	return true;
}

int AudioDriverClass::getFrequency()
{
	return s_AudioFrequency;
}

void AudioDriverClass::beginBuffer(short **buffer, int *samples)
{
	buffer = NULL;
	samples = 0;
}

void AudioDriverClass::endBuffer()
{

}

void AudioDriverClass::end()
{
	SDL_CloseAudio();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

} /* namespace GDEMU */

#endif /* #ifdef GDEMU_SDL */

/* end of file */
