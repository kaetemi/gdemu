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

SystemSdlClass SystemSdl;

void SystemSdlClass::ErrorSdl()
{
	printf("ErrorSdl: %s", SDL_GetError());
	exit(1);
}

} /* namespace GDEMU */

#endif /* #ifdef GDEMU_SDL */

/* end of file */
