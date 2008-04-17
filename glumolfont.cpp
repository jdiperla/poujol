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

#include "stdafx.h"
#include "glumolfont.h"
#include "glumolgame.h"

#include <list>
#include <string>

extern CGlumolGame *CurrentGame;

CGlumolFont::CGlumolFont() :
    CL_Font()
{
    font = NULL;
}

CGlumolFont::~CGlumolFont(void)
{
   if(font) delete font;
}

CGlumolFont::CGlumolFont(const char *fontname, int height, int width, bool bold, bool italic, bool underline, bool strikeout)
	: CL_Font() {
    font = NULL;
    CL_Font::operator =(CL_Font(fontname, height, width, bold, italic, underline, strikeout));
}

void CGlumolFont::add_letters(const char *letters, const char *filename, std::vector<int> widths)
{
    size_t n = strlen(letters);
    n = min(n, widths.size());
	CL_PixelBuffer pxl_buffer;
    if(CurrentGame->is_file_in_archive(filename))
        pxl_buffer = CL_ProviderFactory::load(filename, "", CurrentGame->get_archive());
	else
		pxl_buffer = CL_ProviderFactory::load(filename);
	CL_SpriteDescription build_desc;
    int x = 0;
    int h = pxl_buffer.get_height();
    for(std::vector<int>::iterator i = widths.begin();
        i != widths.end(); i++) {
        build_desc.add_gridclipped_frames(pxl_buffer, x, 0, (*i), h,
		                                  1, 1, 0, 0);
        x += *i;
    }
    CL_Sprite sprite(build_desc);
    CL_Font::operator =(CL_Font(sprite, letters));
}

void CGlumolFont::draw(const char *text, float x, float y, CL_Sizef max_size, CL_Color color)
{
    set_color(color);
    CL_Font::draw(x, y, text);
}

void CGlumolFont::draw_to_gc(const char *text, int x, int y, CL_GraphicContext *gc, CL_Sizef max_size)
{
    CL_Font::draw(CL_Rect(x, y, x + (int) max_size.width, y + (int) max_size.height), text, gc);
}

CL_Sizef CGlumolFont::get_size(const char *text, CL_Sizef max_size)
{
	CL_Size Max_size = CL_Size((int) max_size.width, (int) max_size.height);
    return CL_Font::get_size(text, Max_size);
}
