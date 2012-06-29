/**
 * pgmspace
 * $Id$
 * \file pgmspace.h
 * \brief pgmspace
 */

/*
 * Copyright (C) 2011  by authors
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

#ifndef PGMSPACE_H
#define PGMSPACE_H
// #include <...>

// System includes

// Project includes

typedef signed char prog_char;
typedef unsigned char prog_uchar;

#define PROGMEM
#define pgm_read_byte_near *
#define pgm_read_byte pgm_read_byte_near
inline unsigned short pgm_read_word_near(char *ptr) { return *((unsigned short *)ptr); }
inline unsigned short pgm_read_word_near(unsigned char *ptr) { return *((unsigned short *)ptr); }
inline unsigned short pgm_read_word_near(short *ptr) { return *((unsigned short *)ptr); }
inline unsigned short pgm_read_word_near(unsigned short *ptr) { return *ptr; }
inline void *pgm_read_word_near(void **ptr) { return *ptr; }
inline prog_uchar *pgm_read_word_near(prog_uchar **ptr) { return *ptr; }
inline prog_char *pgm_read_word_near(prog_char **ptr) { return *ptr; }
#define pgm_read_word pgm_read_word_near
inline unsigned int pgm_read_dword_near(char *ptr) { return *((unsigned short *)ptr); }
inline unsigned int pgm_read_dword_near(unsigned char *ptr) { return *((unsigned short *)ptr); }
inline unsigned int pgm_read_dword_near(int *ptr) { return *((unsigned int *)ptr); }
inline unsigned int pgm_read_dword_near(unsigned int *ptr) { return *ptr; }
#define pgm_read_dword pgm_read_dword_near

#endif /* #ifndef PGMSPACE_H */

/* end of file */
