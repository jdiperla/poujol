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

#ifndef _MISC_CLASSES_H_
#define _MISC_CLASSES_H_

#include <ClanLib/core.h>
#include <ClanLib/display.h>

struct sconsole
{
	bool bexist;
	const char* title;
	CL_Sizef size;
	CL_ConsoleWindow* h_console;

};

class console_stdout
{
public :
	sconsole console;

	console_stdout();
	~console_stdout();

	void redirect_std();
	void create();
	void end();
	void write(const char* msg);
 };

CL_Rect To_CL_Rect(const CL_Rectf& f);
CL_Point To_CL_Point(CL_Pointf& f);
CL_Size To_CL_Size(CL_Sizef& f);

enum rotation_dir
{
	clockwise,
	counterclockwise
};

enum play_mode {
	loop,
	pingpong
};

#endif
