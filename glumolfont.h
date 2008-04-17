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

#ifndef _GLUMOLFONT_H_
#define _GLUMOLFONT_H_

#include <ClanLib/Display/font.h>

class CGlumolFont :
	public CL_Font
{
public:
	CL_Font *font;
	std::string font_name;

    CGlumolFont();

	CGlumolFont(const char *font_name,
	            int height,
	            int width = 0,
	            bool bold = false,
	            bool italic = false,
	            bool underline = false,
	            bool strikeout = false);

	virtual ~CGlumolFont(void);

    void add_letters(const char *letters, const char *filename, std::vector<int> widths);
	
    void draw(const char *text,
	          float x, float y,
	          CL_Sizef max_size = CL_Sizef(0, 0),
	          CL_Color color = CL_Color(255, 255, 255, 255));

	void draw_to_gc(const char *text,
	                int x,
	                int y,
	                CL_GraphicContext *gc,
	                CL_Sizef max_size = CL_Sizef(0, 0));

	CL_Sizef get_size(const char *text,
	                  CL_Sizef max_size = CL_Sizef(0, 0));
};

#endif
