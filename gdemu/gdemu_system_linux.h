/**
 * SystemLinuxClass
 * $Id$
 * \file gdemu_system_linux.h
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
#ifndef GDEMU_SYSTEM_LINUX_H
#define GDEMU_SYSTEM_LINUX_H
// #include <...>

// Linux Headers

// SDL
#ifdef GDEMU_SDL
#	include <SDL.h>
#endif

// C Headers
#include <cstdlib>

// STL Headers
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>

namespace GDEMU {

/**
 * SystemLinuxClass
 * \brief SystemLinuxClass
 * \date 2012-06-29 14:50GMT
 * \author Jan Boon (Kaetemi)
 */
class SystemLinuxClass
{
public:
	SystemLinuxClass() { }

	static void Error(char *message);

private:
	SystemLinuxClass(const SystemLinuxClass &);
	SystemLinuxClass &operator=(const SystemLinuxClass &);

}; /* class SystemLinuxClass */

extern SystemLinuxClass SystemLinux;

} /* namespace GDEMU */

#endif /* #ifndef GDEMU_SYSTEM_LINUX_H */
#endif /* #ifndef WIN32 */

/* end of file */
