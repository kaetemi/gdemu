/**
 * AudioMachineClass
 * $Id$
 * \file gdemu_audio_machine.h
 * \brief AudioMachineClass
 * \date 2011-05-29 19:43GMT
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

#ifndef GDEMU_AUDIO_MACHINE_H
#define GDEMU_AUDIO_MACHINE_H
// #include <...>

// System includes

// Project includes

namespace GDEMU {

/**
 * AudioMachineClass
 * \brief AudioMachineClass
 * \date 2011-05-29 19:43GMT
 * \author Jan Boon (Kaetemi)
 */
class AudioMachineClass
{
public:
	AudioMachineClass() { }

	static void process();

private:
	AudioMachineClass(const AudioMachineClass &);
	AudioMachineClass &operator=(const AudioMachineClass &);
	
}; /* class AudioMachineClass */

extern AudioMachineClass AudioMachine;

} /* namespace GDEMU */

#endif /* #ifndef GDEMU_AUDIO_MACHINE_H */

/* end of file */
