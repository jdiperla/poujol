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
#include "glumolzbuffer.h"
#include "glumolgame.h"

CZBuffer::CZBuffer(CGlumolSprite *parent)
	: CGlumolSprite(parent)
{
}

void CZBuffer::on_draw()
{
	CL_GraphicContext *gc = CurrentGame->window.h_window->get_gc();
	if(!CurrentGame->stencil_test)
	   gc->clear_stencil();
	CurrentGame->stencil_test = true;
	gc->enable_stencil_write(true, false); 
	CGlumolSprite::on_draw();
	gc->enable_stencil_write(false);
	gc->enable_stencil_test(true);
}

CZBuffer::~CZBuffer()
{
}
