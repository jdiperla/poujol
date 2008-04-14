#ifndef _GLUMOLANIMATION_H_
#define _GLUMOLANIMATION_H_

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <vector>
#include <string>
#include "poujol.h"

#ifdef USE_BOOST
#include <boost/shared_ptr.hpp>
#endif

class CGlumolRegion;

typedef std::vector<SHARED_PTR(CGlumolRegion)> Regions;

struct Times : public std::vector<float>
{
	Times()
	{}

	Times(std::vector<float>::iterator it1, std::vector<float>::iterator it2)
	{
	}
};

struct Orders : public std::vector<int>
{
	Orders()
	{}

	Orders(std::vector<int>::iterator it1, std::vector<int>::iterator it2)
	{
	}
};

struct Hotspots : public std::vector<CL_Pointf>
{
	Hotspots()
	{}

	Hotspots(std::vector<CL_Pointf>::iterator it1, std::vector<CL_Pointf>::iterator it2)
	{
	}
};

struct MoveOffsets: public std::vector<CL_Pointf>
{
	MoveOffsets()
	{}

	MoveOffsets(std::vector<CL_Pointf>::iterator it1, std::vector<CL_Pointf>::iterator it2)
	{
	}
};

class CGlumolAnimation
{
public:
	enum format { autodetect, rgba, rgb, rgb565, paletted };

	CGlumolAnimation();
	~CGlumolAnimation();

	Times &get_times() { return times; }
	Hotspots &get_hotspots() { return hotspots; }
	MoveOffsets &get_move_offsets() { return move_offsets; }
	Orders &get_orders() { return orders; }
	CL_Sprite* get_sprite() { return tmp_sprite; }
	CL_PixelBuffer get_buffer() { return pxl_buffer; }
	int get_nbframes() { return nbframes; }

	CL_Sizef frame_size;
	CL_PixelBuffer pxl_buffer;
	CL_Sprite* tmp_sprite;
    std::vector<CL_PixelBuffer> pxl_buffers;
	std::string name;
	Times times;
	Hotspots hotspots;
	MoveOffsets move_offsets;
	Orders orders;
    CL_SpriteDescription build_desc;
	int nbframes;
	int virtual_frames;
	int current_order;
	int alpha_mask, red_mask, green_mask, blue_mask;
};

class CBandeAnimation : public CGlumolAnimation
{
public:
	CBandeAnimation(const char* name, CL_Color color, int nbframes,
	                int virtual_frames = -1, format = CGlumolAnimation::autodetect);
	CBandeAnimation(const char* name, int nbframes, int virtual_frames = -1,
	                format = CGlumolAnimation::autodetect);
	//CBandeAnimation(int nbframes, int w, int h, int virtual_frames = -1,
	//                format = CGlumolAnimation::autodetect);

    virtual ~CBandeAnimation();
};

class CAnimation : public CGlumolAnimation
{
public:
	CAnimation();

    void add_frame(const char *filename, CL_Color transparent = CL_Color(0,0,0), CGlumolAnimation::format = CGlumolAnimation::autodetect);
    void build();
};

#endif
