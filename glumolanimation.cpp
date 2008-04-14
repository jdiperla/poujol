#include "stdafx.h"
#include "misc_classes.h"
#include "glumolanimation.h"
#include "glumolgame.h"
#include "glumolexception.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/Display/Providers/provider_factory.h>

CGlumolAnimation::CGlumolAnimation()
{
}

CGlumolAnimation::~CGlumolAnimation()
{
#ifdef TRACE_DELETIONS
	std::cout << "CBandeAnimation::~CBandeAnimation " << name << std::endl;
#endif
	if(tmp_sprite)
		delete tmp_sprite;
}

CBandeAnimation::CBandeAnimation(const char* name, int frames, int virtual_frames, format f)
{
	this->name = name;
	pxl_buffer = NULL;
	tmp_sprite = NULL;
	nbframes = frames;

	if(nbframes < 1 || !CurrentGame)
		return;

	if(virtual_frames == -1)
		virtual_frames = frames;

	if(CurrentGame->screen == NULL)
		throw CL_Error("You must create a display window before loading graphics.");

	try {
		if(CurrentGame->is_file_in_archive(name))
			pxl_buffer = CL_ProviderFactory::load(name, "", CurrentGame->get_archive());
		else
			pxl_buffer = CL_ProviderFactory::load(name);

        pxl_buffers.push_back(pxl_buffer);

        // *pxl_buffer = pxl_buffer.to_format(CL_PixelFormat::rgb565);
		pxl_buffer.lock();

		alpha_mask = pxl_buffer.get_format().get_alpha_mask();
		red_mask = pxl_buffer.get_format().get_red_mask();
		green_mask = pxl_buffer.get_format().get_green_mask();
		blue_mask = pxl_buffer.get_format().get_blue_mask();

        int height  = pxl_buffer.get_height();
		int width = pxl_buffer.get_width() / nbframes;

		frame_size = CL_Sizef((float) width, (float) height);

		build_desc.add_gridclipped_frames(pxl_buffer, 0, 0, width, height,
			nbframes, 1, 0, 0);

		times.resize(virtual_frames, 100);
		orders.resize(virtual_frames);
		hotspots.resize(virtual_frames, CL_Pointf(0, 0));
		move_offsets.resize(virtual_frames, CL_Pointf(0, 0));

		for(int i = 0; i < virtual_frames; i++) {
			orders[i] = i % frames;
		}

		tmp_sprite = new CL_Sprite(build_desc);
		pxl_buffer.unlock();
	}

	catch (CL_Error error) {
		std::cout << "Error while loading " << name << " : " << error.message.c_str() << std::endl;
	}
}

CBandeAnimation::CBandeAnimation(const char* name, CL_Color transparent, int frames, int virtual_frames, format f)
{
	this->name = name;
	pxl_buffer = NULL;
	tmp_sprite = NULL;
	nbframes = frames;

	if(nbframes < 1 || !CurrentGame)
		return;

	if(virtual_frames == -1)
		virtual_frames = frames;

	if(CurrentGame->screen == NULL)
		throw CL_Error("You must create a display window before loading graphics.");

	try {
		if(CurrentGame->is_file_in_archive(name))
			pxl_buffer = CL_ProviderFactory::load(name, "", CurrentGame->get_archive());
		else
			pxl_buffer = CL_ProviderFactory::load(name);

                transparent.set_alpha(0);
                // *pxl_buffer = pxl_buffer.to_format(CL_PixelFormat::rgba4444);
                pxl_buffer.set_colorkey(true, transparent.color);

		pxl_buffers.push_back(pxl_buffer);

        pxl_buffer.lock();

		alpha_mask = pxl_buffer.get_format().get_alpha_mask();
		red_mask = pxl_buffer.get_format().get_red_mask();
		green_mask = pxl_buffer.get_format().get_green_mask();
		blue_mask = pxl_buffer.get_format().get_blue_mask();

        int height  = pxl_buffer.get_height();
		int width = pxl_buffer.get_width() / nbframes;

		frame_size = CL_Sizef((float) width, (float) height);

		build_desc.add_gridclipped_frames(pxl_buffer, 0, 0, width, height,
			nbframes, 1, 0, 0);

		times.resize(virtual_frames, 100);
		orders.resize(virtual_frames);
		hotspots.resize(virtual_frames, CL_Pointf(0, 0));
		move_offsets.resize(virtual_frames, CL_Pointf(0, 0));

		for(int i = 0; i < virtual_frames; i++) {
			orders[i] = i % frames;
		}

		tmp_sprite = new CL_Sprite(build_desc);
		pxl_buffer.unlock();
	}

	catch (CL_Error error) {
		std::cout << "Error while loading " << name << " : " << error.message.c_str() << std::endl;
	}
}

CBandeAnimation::~CBandeAnimation()
{
}

CAnimation::CAnimation()
{
    this->name = "Unnamed animation";
    pxl_buffer = NULL;
    tmp_sprite = NULL;
    nbframes = 0;
}

void CAnimation::add_frame(const char *filename, CL_Color transparent, format f)
{
    nbframes++;
    virtual_frames++;

	try {
        if(CurrentGame && CurrentGame->is_file_in_archive(filename))
			pxl_buffer = CL_ProviderFactory::load(filename, "", CurrentGame->get_archive());
		else
            pxl_buffer = CL_ProviderFactory::load(filename);
#ifdef _WIN32_WCE
		pxl_buffer = pxl_buffer.to_format(CL_PixelFormat::rgba4444);
#endif
		pxl_buffers.push_back(pxl_buffer);
	}
	catch(...) {
		throw GlumolException(std::string("Cannot find the filename ") + filename);
	}

    if(!pxl_buffer.get_format().has_colorkey()) {
        transparent.set_alpha(0);
        pxl_buffer.set_colorkey(true, transparent.color);
    }
    pxl_buffer.lock();

	alpha_mask = pxl_buffer.get_format().get_alpha_mask();
	red_mask = pxl_buffer.get_format().get_red_mask();
	green_mask = pxl_buffer.get_format().get_green_mask();
	blue_mask = pxl_buffer.get_format().get_blue_mask();

    int height  = pxl_buffer.get_height();
    int width = pxl_buffer.get_width();

    frame_size = CL_Sizef((float) width, (float) height);

    build_desc.add_gridclipped_frames(pxl_buffer, 0, 0,
                                      width, height, 1, 1, 0, 0);

    pxl_buffer.unlock();
}

void CAnimation::build()
{
    tmp_sprite = new CL_Sprite(build_desc, true);

    times.resize(nbframes, 100);
    orders.resize(nbframes);
    hotspots.resize(nbframes, CL_Pointf(0, 0));
    move_offsets.resize(nbframes, CL_Pointf(0, 0));

    for(int i = 0; i < nbframes; i++) {
        orders[i] = i;
    }
}
