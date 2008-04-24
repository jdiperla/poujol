/*
Glumol - An adventure game creator
Copyright (C) 1998-2008  Sylvain Baubeau & Alexis Contour

This file is part of Glumol.

Glumol is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

Glumol is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Glumol.  If not, see <http://www.gnu.org/licenses/>.
*/

#define DEFAULT_SCREEN_WIDTH  640
#define DEFAULT_SCREEN_HEIGHT 480
#define DEFAULT_SCREEN_DEPTH   32

#ifdef UNICODE
# define DEFAULT_SCREEN_TITLE _T("Glumol")
#else
# define DEFAULT_SCREEN_TITLE "Glumol"
#endif

#define USE_GUI
#define USE_GL
#define USE_BOOST
#define SHOW_CONSOLE
#define USE_SOUND
#define USE_VORBIS
#define USE_MIKMOD

