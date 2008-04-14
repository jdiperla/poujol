#ifndef GLUMOLZBUFFER_H
#define GLUMOLZBUFFER_H

#include "glumolsprite.h"

class CZBuffer : public CGlumolSprite
{
public:
	CZBuffer(CGlumolSprite *parent);
	~CZBuffer();

	virtual void on_draw();
};

#endif
