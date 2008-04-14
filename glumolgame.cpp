#include "stdafx.h"

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include "poujol.h"

#define CLEAN_FOR_RERUN
#define DEBUG_FOCUS 0

#define USE_MIKMOD

#ifdef USE_GL
	#include <ClanLib/gl.h>
	
	#ifdef CLEAN_FOR_RERUN
	   #include <GL/opengl_state_generic.h>
    #endif
    
	#if defined(WIN32)
		#include <GL/WGL/display_window_opengl.h>
	#elif defined(__APPLE__)
	    #include <GL/AGL/display_window_opengl.h>
	#else
	    #include <GL/GLX/display_window_opengl.h>
	#endif
#endif

#include <ClanLib/sdl.h>
#include <ClanLib/sound.h>
#include <ClanLib/vorbis.h>
#include <ClanLib/gui.h>
#include <ClanLib/mikmod.h>
#include "misc_classes.h"
#include "glumolgame.h"
#include "cl_writepng.h"

#if defined(_WINDOWS) || defined(_WIN32_WCE)
HINSTANCE hinstance = NULL;
#endif

CGlumolScreen* the_screen = NULL;
bool inited = false;

CGlumolGame::CGlumolGame(bool _fullscreen, int width, int height,
                         int depth, const gstring& title, bool _use_sdl,
                         bool can_resize)
{
	fullscreen = _fullscreen;
	screen_width = width;
    screen_height = height;
    screen_depth = depth;
	window.title = title;
    allow_resize = can_resize;
    use_sdl = _use_sdl;
    on_init();
}

void CGlumolGame::on_init()
{
#if defined(_WINDOWS) || defined(_WIN32_WCE)
	CL_SetupCore::set_instance(hinstance);
#endif
	CurrentGame = this;
	stencil_test = false;
	bexit = false;
	archive = NULL;
	cursor_pos = CL_Pointf(0,0);
	screen = new CGlumolScreen();
	the_screen = screen;
	current_focused_obj = SHARED_PTR_FOO(CGlumolSprite);

#if defined(USE_GL) && defined(CLEAN_FOR_RERUN)
	#ifndef __APPLE__
	CL_OpenGLState_Generic::cur_drawable = 0;
	CL_OpenGLState_Generic::cur_context = 0;
	#endif
# if defined(_WINDOWS) || defined(WIN32) || defined(WINDOWS)
	CL_DisplayWindow_OpenGL::opengl_contexts.clear();
# elif defined(__APPLE__)
    CL_DisplayWindow_OpenGL::share_context = 0;
    CL_DisplayWindow_OpenGL::disp_ref_count = 0;
# else
    CL_DisplayWindow_OpenGL::share_context = 0;
    CL_DisplayWindow_OpenGL::disp = 0;
    CL_DisplayWindow_OpenGL::disp_ref_count = 0;
# endif
	CL_OpenGLState_Generic::function_map.clear();
#endif

	window.bexist = false;
	window.h_window = NULL;
	window.position = CL_Pointf(50,50);

	filename = "";

#ifdef SHOW_CONSOLE
	console = new console_stdout;
#endif

	if(!inited) {
		CL_SetupCore::init();
		CL_SetupDisplay::init();

        if(use_sdl) {
    		CL_SetupSDL::init();
        }
        else {
#ifdef USE_GL
    		CL_SetupGL::init();
#else
    		CL_SetupSDL::init();
#endif
        }
		modes = CL_DisplayMode::get_display_modes();
#ifdef USE_GUI
		CL_SetupGUI::init();
#endif
#ifdef USE_SOUND
		CL_SetupSound::init();
        output = CL_SoundOutput(44100);
		CL_Sound::select_output(&output);
#endif
#ifdef USE_MIKMOD
        CL_SetupMikMod::init();
#endif
#ifdef USE_VORBIS
		CL_SetupVorbis::init();
#endif
        inited = true;
	}
	window.h_window = new CL_DisplayWindow(
                          window.title, (int) screen_width, 
                          (int) screen_height, fullscreen, allow_resize);
	window.bexist = true;
	mouse_sprite = SHARED_PTR_FOO(CGlumolSprite);
	input_dispatcher = new CGlumolInputDispatcher(this);
    if(allow_resize) {
        slots.connect(window.h_window->sig_resize(), this, &CGlumolGame::on_resize_signal);
        slots.connect(window.h_window->sig_window_close(), this, &CGlumolGame::on_close_signal);
    }
}

const std::vector < CL_DisplayMode > &CGlumolGame::get_display_modes()
{
	return modes;
}

void CGlumolGame::on_close()
{
}

void CGlumolGame::on_close_signal()
{
    on_close();
}

void CGlumolGame::on_resize(int width, int height)
{
}

void CGlumolGame::on_resize_signal(int width, int height)
{
    on_resize(width, height);
    screen_width = width;
    screen_height = height;
}

CGlumolGame::~CGlumolGame()
{
#ifdef TRACE_DELETIONS
	std::cout << "DELETE GAME" << std::endl;
#endif
	quit();
#ifdef TRACE_DELETIONS
	std::cout << "GAME DELETED" << std::endl;
#endif
}

void  CGlumolGame::set_archive_filename(const char* name)
{
	filename = name;
	archive = new CL_Zip_Archive(name);
	if(!archive){
		throw CL_Error(std::string("cannot open the given file ") + name);
	}
}

SHARED_PTR(CGlumolSprite) CGlumolGame::get_current_focused_obj_in(SHARED_PTR(CGlumolSprite) sprite, int x, int y)
{
	return SHARED_PTR_FOO(CGlumolSprite);
}

bool CGlumolGame::is_mode_available(CL_Size mode)
{
	for(unsigned int i = 0; i < modes.size(); ++i)
	{
		if (modes[i].get_resolution() == mode) 
			return true; 
	}
	return false; 
}

bool CGlumolGame::is_file_in_archive(const char* name)
{
	if(get_archive())
	{
		std::vector<CL_Zip_FileEntry>& filelist = CurrentGame->get_archive()->get_file_list();
		for(std::vector<CL_Zip_FileEntry>::iterator i = filelist.begin(); i != filelist.end(); i++) 
		{
			if((*i).get_filename() ==  name )
				return true;
		}
		return false;
	}
	return false; 
	
}

void CGlumolGame::set_size(float w, float h)
{
	if(h <= 0.0f) h = 1.0f;
	if(w <= 0.0f) w = 1.0;
    CL_Sizef request_size(w, h);

    if(fullscreen)
		if(!is_mode_available(To_CL_Size(request_size)))
			set_fullscreen(false);

	CL_Display::set_size((int)request_size.width, (int)request_size.height);
	screen_width = w;
	screen_height = h;
}

void CGlumolGame::set_fullscreen(bool f)
{
	if(f == fullscreen)
		return;
	if(f) {
		CL_Sizef s = CL_Sizef(screen_width, screen_height);
		if (is_mode_available(To_CL_Size(s)))
			CL_Display::set_fullscreen((int) get_width(),(int) get_height(), screen_depth);
		else return;
	}
	else
		CL_Display::set_windowed();

	fullscreen = f;
}

void CGlumolGame::set_window_title(GLUMOLSTRING title)
{
	if(window.bexist)
		window.h_window->set_title(title);
	window.title = title;
}

GLUMOLSTRING CGlumolGame::get_window_title()
{
	return window.title;
}

int CGlumolGame::get_framerate()
{
    return framerate.get_fps();
}

SHARED_PTR(CGlumolSprite) CGlumolGame::get_current_focused_obj(int x, int y )
{
	SHARED_PTR(CGlumolSprite) s = screen->get_focused_object(x,y);
#if DEBUG_FOCUS
    std::cout << "get_focused_object " << s << std::endl;
#endif
	return s;
}

void CGlumolGame::show_console(bool state)
{
	if (state)
		console->create();
	else
		console->end();
}

bool CGlumolGame::is_console_visible()
{
	return console->console.bexist;
}

console_stdout* CGlumolGame::get_console()
{
	return console;
}

void CGlumolGame::set_window_position(CL_Pointf point)
{
	window.position = point;
	if(window.bexist)
		CL_Display::set_position((int) window.position.x,
                                 (int) window.position.y);
}

CL_Pointf CGlumolGame::get_window_position()
{
	return window.position;
}

int CGlumolGame::run()
{
	try {
		on_main();
	}
	catch(CL_Error error) {
		std::cout << "Exception caught : " << error.message.c_str() << std::endl;
		return -1;
	}
	return 0;
}

void CGlumolGame::update()
{
    update_inputs();
    the_screen->update();

    on_draw();

#ifdef USE_GUI
    for(std::list<CL_GUIManager*>::iterator i = guis.begin(); i != guis.end(); i++) {
        std::cout << "show gui" << std::endl;
        (*i)->show();
    }
#endif

    CL_Display::flip();
    CL_System::keep_alive(1);
}

void CGlumolGame::quit()
{
	if(bexit) return;
	bexit = true;
    window.h_window->show_cursor();
	current_focused_obj = SHARED_PTR_FOO(CGlumolSprite);

	if(is_console_visible()) {
		this->show_console(false);
		console->end();
	}

	if(input_dispatcher)
		delete input_dispatcher;

	mouse_sprite = SHARED_PTR_FOO(CGlumolSprite);

	if(screen) {
		delete screen;
		screen = NULL;
	}

	if(window.bexist) {
		delete window.h_window;
		window.bexist = false;
	}
	if(CurrentGame != this) return;

	CurrentGame = NULL;

	if(inited) {
        if(use_sdl) {
    		CL_SetupSDL::deinit();
        }
        else {
#ifdef USE_GL
    		CL_SetupGL::deinit();
#else
    		CL_SetupSDL::deinit();
#endif
        }
		CL_SetupDisplay::deinit();
		CL_SetupCore::deinit();
#ifdef USE_SOUND
		CL_SetupSound::deinit();
#endif
#ifdef USE_VORBIS
		CL_SetupVorbis::deinit();
#endif
#ifdef USE_GUI
	    CL_SetupGUI::deinit();
#endif
#ifdef USE_MIKMOD
        CL_SetupMikMod::deinit();
#endif
		inited = false;
	}
}

void CGlumolGame::on_key_down(int key)
{
    if(current_focused_obj) {
	    current_focused_obj->on_key_down(key & 0xFF);
	}
}

void CGlumolGame::on_key_up(int key)
{
	if(current_focused_obj) {
		 current_focused_obj->on_key_up(key & 0xFF);
	}
}

void CGlumolGame::on_key_down_repeat(int key, unsigned int milliseconds)
{
	if(current_focused_obj) {
		current_focused_obj->on_key_down(key & 0xFF);
	}
}

void CGlumolGame::on_left_button_down()
{
	if(current_focused_obj) {
		current_focused_obj->on_left_button_down();
	}
}

void CGlumolGame::on_right_button_down()
{
	if(current_focused_obj) {
		current_focused_obj->on_right_button_down();
	}
}

void CGlumolGame::on_mouse_wheel_up()
{
	if(current_focused_obj) {
		current_focused_obj->on_mouse_wheel_up();
	}
}

void CGlumolGame::on_mouse_wheel_down()
{
	 if(current_focused_obj) {
		 current_focused_obj->on_mouse_wheel_down();
	 }
}

void CGlumolGame::on_left_button_up()
{
	 if(current_focused_obj) {
		 current_focused_obj->on_left_button_up();
	 }
}

void CGlumolGame::on_right_button_up()
{
	if(current_focused_obj) {
	    current_focused_obj->on_right_button_up();
	}
}

void CGlumolGame::on_middle_button_down()
{
	if(current_focused_obj) {
	    current_focused_obj->on_middle_button_down();
	}
}

void CGlumolGame::on_middle_button_up()
{
	 if(current_focused_obj) {
		 current_focused_obj->on_middle_button_up();
	 }
}

void CGlumolGame::on_mouse_move(CL_Pointf oldPos, CL_Pointf newPos, float axis)
{
	if(mouse_sprite) {
		mouse_sprite->set_position(newPos);
		if(current_focused_obj != NULL) {
			current_focused_obj->on_mouse_move(oldPos, newPos, axis);
		}
	}
}

void CGlumolGame::on_left_button_down_repeat(unsigned int milliseconds)
{
	if(current_focused_obj) {
		current_focused_obj->on_left_button_down_repeat(milliseconds);
	}
}

void CGlumolGame::on_right_button_down_repeat(unsigned int milliseconds)
{
	if(current_focused_obj) {
		 current_focused_obj->on_right_button_down_repeat(milliseconds);
	}
}

void CGlumolGame::on_middle_button_down_repeat(unsigned int milliseconds)
{
	if(current_focused_obj) {
		current_focused_obj->on_middle_button_down_repeat(milliseconds);
	}
}

void CGlumolGame::on_mouse_button_up(unsigned int button)
{
	switch(button) {
	case 0:
		on_left_button_up();
		break;
	case 1:
		on_right_button_up();
		break;
	case 2:
		on_middle_button_up();
		break;
	default :
		break;
	}
	if(current_focused_obj && button <3 ) {
		 current_focused_obj->on_mouse_button_up(button);
	}
}


void CGlumolGame::on_mouse_button_down(unsigned int button)
{
	switch(button)
	{
	case 0 :
		on_left_button_down();
		break;
	case 1 :
		on_right_button_down();
		break;
	case 2:
		on_middle_button_down();
		break;
	case 3:
		on_mouse_wheel_up();
		break;
	case 4:
		on_mouse_wheel_down();
		break;
	default:
		break;
	}
	if(current_focused_obj) {
		current_focused_obj->on_mouse_button_down(button);
	}
}

void CGlumolGame::take_screenshot(const std::string& filename)
{
	CL_PixelBuffer back_buffer = CL_Display::get_current_window()->get_back_buffer();
	
    back_buffer.lock();

	unsigned short width = back_buffer.get_width();
	unsigned short height = back_buffer.get_height();
	CL_PixelBuffer pbuf(width, height, width*3, CL_PixelFormat::rgba8888);
	back_buffer.convert(pbuf);

    back_buffer.unlock();
    WritePNG::write_png(pbuf, filename);
}

void CGlumolGame::update_inputs()
{
	if ( ((CL_Mouse::get_x() > 0) &&  (CL_Mouse::get_y() > 0)) ) {
		SHARED_PTR(CGlumolSprite) obj = get_current_focused_obj(CL_Mouse::get_x(), CL_Mouse::get_y());

		if (obj != current_focused_obj)	{
			if(obj) {
				obj->on_focus(current_focused_obj);
			}
			if(current_focused_obj) {
				current_focused_obj->on_lose_focus(obj);
			}
			current_focused_obj = obj ;
		}
	}
	input_dispatcher->DispatchRepeat();
}

float CGlumolGame::get_mouse_sensibility()
{
	return 0;
}

void CGlumolGame::set_mouse_sensibility(float s)
{
}

float CGlumolGame::get_mouse_acceleration()
{
	return 0;
}

void CGlumolGame::set_mouse_acceleration(float a)
{
}

bool CGlumolGame::get_enable_mouse()
{
	return true;
}

void CGlumolGame::set_enable_mouse(bool e)
{
}

void CGlumolGame::set_mouse_sprite(SHARED_PTR(CGlumolSprite) sprite)
{
	mouse_sprite = sprite;
	if(mouse_sprite)
		mouse_sprite->set_position(get_mouse_position());

	CurrentGame->window.h_window->hide_cursor();
}

SHARED_PTR(CGlumolSprite) CGlumolGame::get_mouse_sprite()
{
	return mouse_sprite;
}

void CGlumolGame::set_mouse_position(CL_Pointf pt)
{
	CL_Mouse::set_position((int) pt.x, (int) pt.y);
}

CL_Pointf CGlumolGame::get_mouse_position()
{
	return CL_Pointf((float) CL_Mouse::get_x(), (float) CL_Mouse::get_y());
}

CGlumolGame *get_game()
{
   return CurrentGame;
}

void set_game(CGlumolGame *game)
{
   CurrentGame = game;
}
