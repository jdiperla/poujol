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
