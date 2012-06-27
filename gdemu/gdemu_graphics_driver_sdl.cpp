/**
 * GraphicsDriverClass
 * $Id$
 * \file gdemu_graphics_driver_sdl.cpp
 * \brief GraphicsDriverClass
 * \date 2012-06-27 11:49GMT
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
#include "gdemu_graphics_driver.h"

// System includes
#include "gdemu_system.h"
#include "gdemu_system_sdl.h"
#include "gdemu_gameduino_spi.h"

// Project includes

using namespace std;

#define GDEMU_WINDOW_TITLE "Gameduino Emulator"
#define GDEMU_WINDOW_WIDTH 400
#define GDEMU_WINDOW_HEIGHT 300
#define GDEMU_WINDOW_RATIO (4.0f / 3.0f)
#define GDEMU_WINDOW_KEEPRATIO 1

namespace GDEMU {


GraphicsDriverClass GraphicsDriver;
static argb1555 s_BufferARGB1555[GDEMU_WINDOW_WIDTH * GDEMU_WINDOW_HEIGHT];

SDL_Surface *s_Screen = NULL;
SDL_Surface *s_Buffer = NULL;

argb1555 *GraphicsDriverClass::getBufferARGB1555()
{
	return s_BufferARGB1555;
}

void GraphicsDriverClass::begin()
{
	SDL_InitSubSystem(SDL_INIT_VIDEO);

	s_Screen = SDL_SetVideoMode(GDEMU_WINDOW_WIDTH * 2, GDEMU_WINDOW_HEIGHT * 2, 15, SDL_SWSURFACE);
	if (s_Screen == NULL) SystemSdlClass::ErrorSdl();

	SDL_WM_SetCaption(GDEMU_WINDOW_TITLE, NULL);

	Uint32 bpp;
	Uint32 rmask, gmask, bmask, amask;

	rmask = 0x001F;
	gmask = 0x03E0;
	bmask = 0x7C00;
	amask = 0x0000;

	bpp = 15;

	s_Buffer = SDL_CreateRGBSurfaceFrom(s_BufferARGB1555, GDEMU_WINDOW_WIDTH, GDEMU_WINDOW_HEIGHT, bpp, 2 * GDEMU_WINDOW_WIDTH, rmask, gmask, bmask, amask);
	if (s_Buffer == NULL) SystemSdlClass::ErrorSdl();
}

bool GraphicsDriverClass::update()
{
	SDL_Event event;

	while ( SDL_PollEvent(&event) ) {
		switch (event.type) {
			// don't care about other events
			case SDL_QUIT:
				return false;
		}
	}
	return true;
}

void GraphicsDriverClass::end()
{
	// ... TODO ...

	SDL_FreeSurface(s_Buffer);
	SDL_FreeSurface(s_Screen);

	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void GraphicsDriverClass::renderBuffer()
{
	// TODO: Allow resize and aspect ratio

	/*SDL_Rect destRect;
	destRect.x = 0;
	destRect.y = GDEMU_WINDOW_HEIGHT * 2;
	destRect.w = GDEMU_WINDOW_WIDTH * 2;
	destRect.h = GDEMU_WINDOW_HEIGHT * -2;*/

	if (SDL_SoftStretch(s_Buffer, NULL, s_Screen, NULL) < 0)
		SystemSdlClass::ErrorSdl();

	if (SDL_Flip(s_Screen) < 0)
		SystemSdlClass::ErrorSdl();

	/*
	// Render bitmap to buffer
#if !GDEMU_GRAPHICS_USE_STRETCHDIBITS
	if (!SetDIBitsToDevice(s_HDC, 0, 0,
		GDEMU_WINDOW_WIDTH, GDEMU_WINDOW_HEIGHT,
		0, 0, 0, GDEMU_WINDOW_HEIGHT, s_BufferARGB1555, &s_BitInfo, DIB_RGB_COLORS))
		SystemWindows.Error(TEXT("SetDIBitsToDevice  FAILED"));
#endif

	// Draw buffer to screen
	RECT r;
	GetClientRect(s_HWnd, &r);
#if GDEMU_WINDOW_KEEPRATIO
	{
		argb1555 bgC16 = ((argb1555 *)(void *)(&GameduinoSPI.getRam()[0x280e]))[0];
		COLORREF bgC32 = RGB((((bgC16) & 0x1F) * 255 / 31),
			(((bgC16 >> 5) & 0x1F) * 255 / 31),
			(((bgC16 >> 10) & 0x1F) * 255 / 31));
		HBRUSH bgBrush = CreateSolidBrush(bgC32);
		if (bgBrush == NULL) SystemWindows.ErrorWin32();
		int width_r = (int)((float)r.bottom * GDEMU_WINDOW_RATIO); int height_r;
		if (width_r > r.right) { width_r = r.right; height_r = (int)((float)r.right / GDEMU_WINDOW_RATIO); }
		else height_r = r.bottom;
		int x_r = (r.right - width_r) / 2;
		int y_r = (r.bottom - height_r) / 2;
		HDC hdc = GetDC(s_HWnd);
#if !GDEMU_GRAPHICS_USE_STRETCHDIBITS
		StretchBlt(hdc, x_r, y_r, width_r, height_r, s_HDC, 0, 0, GDEMU_WINDOW_WIDTH, GDEMU_WINDOW_HEIGHT, SRCCOPY);
#else
		StretchDIBits(hdc, x_r, y_r, width_r, height_r,	0, 0, GDEMU_WINDOW_WIDTH, GDEMU_WINDOW_HEIGHT, s_BufferARGB1555, &s_BitInfo, DIB_RGB_COLORS, SRCCOPY);
#endif
		RECT rect;
		if (x_r > 0)
		{
			rect.top = 0; rect.left = 0;
			rect.top = 0; rect.left = 0;
			rect.right = (r.right - width_r) / 2;
			rect.bottom = r.bottom;
			FillRect(hdc, &rect, bgBrush); // (HBRUSH)(COLOR_WINDOW + 1));
			rect.left = rect.right + width_r;
			rect.right += rect.left;
			FillRect(hdc, &rect, bgBrush); // (HBRUSH)(COLOR_WINDOW + 1));
		}
		if (y_r > 0)
		{
			rect.top = 0; rect.left = 0;
			rect.right = r.right;
			rect.bottom = (r.bottom - height_r) / 2;
			FillRect(hdc, &rect, bgBrush); // (HBRUSH)(COLOR_WINDOW + 1));
			rect.top = rect.bottom + height_r;
			rect.bottom += rect.top;
			FillRect(hdc, &rect, bgBrush); // (HBRUSH)(COLOR_WINDOW + 1));
		}
		ReleaseDC(s_HWnd, hdc);
		if (!DeleteObject(bgBrush)) SystemWindows.ErrorWin32();
	}
#else
	{
		HDC hdc = GetDC(s_HWnd);
#if !GDEMU_GRAPHICS_USE_STRETCHDIBITS
		StretchBlt(hdc, 0, 0, r.right, r.bottom, s_HDC, 0, 0, GDEMU_WINDOW_WIDTH, GDEMU_WINDOW_HEIGHT, SRCCOPY);
#else
		StretchDIBits(hdc, 0, 0, r.right, r.bottom, 0, 0, GDEMU_WINDOW_WIDTH, GDEMU_WINDOW_HEIGHT, s_BufferARGB1555, &s_BitInfo, DIB_RGB_COLORS, SRCCOPY);
#endif
		ReleaseDC(s_HWnd, hdc);
	}
#endif

	// Update title
	tstringstream newTitle;
	newTitle << GDEMU_WINDOW_TITLE;
	if (GameduinoSPI.getRam()[0x2809] == 0)
		newTitle << TEXT(" [+J1]");
	newTitle << TEXT(" [FPS: ");
	newTitle << System.getFPSSmooth();
	newTitle << TEXT(" (");
	newTitle << System.getFPS();
	newTitle << TEXT(")]");
	SetWindowText(s_HWnd, (LPCTSTR)newTitle.str().c_str());*/
}

} /* namespace GDEMU */

#endif /* #ifdef GDEMU_SDL */

/* end of file */
