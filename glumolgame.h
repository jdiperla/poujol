
#ifndef GLUMOLGAME_H
#define GLUMOLGAME_H

#include <ClanLib/core.h>
#include <ClanLib/sound.h>
#include "glumolsprite.h"
#include "glumolinput.h"
#include "misc_classes.h"
#include "poujol.h"

#ifdef UNICODE
# define GLUMOLSTRING std::wstring
# define gstring      std::wstring
#else
# define GLUMOLSTRING std::string
# define gstring      std::string
#endif

struct swindow
{
	bool bexist;
	GLUMOLSTRING title;
	CL_Pointf position;
	CL_DisplayWindow* h_window ;
};

class CL_GUIManager;

class CGlumolGame
{
public :
	friend class CGlumolInputDispatcher;
	
	CL_Zip_Archive* archive;
	std::list<CL_GUIManager*> guis;
	std::string filename;
	std::vector < CL_DisplayMode > modes;
	SHARED_PTR(CGlumolSprite) current_focused_obj;
	CL_Pointf cursor_pos;
	CGlumolInputDispatcher *input_dispatcher;
	bool bexit;
	CGlumolScreen* screen;
	float screen_width;
	float screen_height;
    int screen_depth;
    bool use_sdl;
	bool fullscreen;
	bool stencil_test;
    bool allow_resize;
	console_stdout* console;
	swindow window;
	CL_FramerateCounter framerate;
    CL_SlotContainer slots;
    CL_SoundOutput output;
#ifdef USE_BOOST
	boost::shared_ptr<CGlumolSprite> mouse_sprite;
#else
	CGlumolSprite *mouse_sprite;
#endif
    
    CGlumolGame(bool _fullscreen = false,
                int width = DEFAULT_SCREEN_WIDTH,
                int height = DEFAULT_SCREEN_HEIGHT,
                int depth = DEFAULT_SCREEN_DEPTH,
                const gstring& title = DEFAULT_SCREEN_TITLE,
                bool use_sdl = false,
                bool can_resize = false);

	virtual ~CGlumolGame(void);

	int run();
	void quit();

	bool is_mode_available(CL_Size mode);

	float get_mouse_sensibility();
	void set_mouse_sensibility(float s);

	CL_Pointf get_mouse_position();
	void set_mouse_position(CL_Pointf pt);

	float get_mouse_acceleration();
	void set_mouse_acceleration(float a);

	bool get_enable_mouse();
	void set_enable_mouse(bool e);

	void set_mouse_sprite(SHARED_PTR(CGlumolSprite) cursor);
	SHARED_PTR(CGlumolSprite) get_mouse_sprite();

	SHARED_PTR(CGlumolSprite) get_current_focused_obj_in(SHARED_PTR(CGlumolSprite), int x, int y);
	SHARED_PTR(CGlumolSprite) get_current_focused_obj(int x, int y);

	void take_screenshot(const std::string& filename);

	const std::vector < CL_DisplayMode > &get_display_modes();

	CL_Zip_Archive* get_archive() { return archive; }

	void add_gui(CL_GUIManager *gui) {
		guis.push_back(gui);
	}

	void remove_gui(CL_GUIManager *gui) {
		guis.remove(gui);
	}

	bool is_file_in_archive(const char* name);

	void set_archive_filename(const char* filename);
	const char* get_archive_filename() { return filename.c_str(); }

	float get_height() { return screen_height; }
	float get_width() { return screen_width; }

    void set_size(float w, float h);

	bool get_fullscreen() { return fullscreen; }
	void set_fullscreen(bool f);

	void show_console(bool state);
	bool is_console_visible();
	console_stdout* get_console();

	void set_window_position(CL_Pointf point);
	CL_Pointf get_window_position();

	void set_window_title(GLUMOLSTRING);
	GLUMOLSTRING get_window_title();

	int get_framerate();

	const CGlumolScreen& get_screen() { return *screen; }

	// GAME INPUTS

	void update();
	void update_inputs();

    virtual void on_close();
    void on_close_signal();
    
    virtual void on_resize(int width, int height);
    void on_resize_signal(int width, int height);

	virtual void on_key_down(int key);
	virtual void on_key_up(int key);
	virtual void on_key_down_repeat(int key, unsigned int miliseconds);

	virtual void on_mouse_button_down(unsigned int button);
	virtual void on_mouse_button_up(unsigned int button);

	virtual void on_left_button_down();
	virtual void on_right_button_down();
	virtual void on_left_button_up();
	virtual void on_right_button_up();

	virtual void on_middle_button_down();
	virtual void on_middle_button_up();

	virtual void on_mouse_wheel_up();
	virtual void on_mouse_wheel_down();

	virtual void on_mouse_move(CL_Pointf, CL_Pointf, float axis);

	virtual void on_left_button_down_repeat(unsigned int milliseconds);
	virtual void on_right_button_down_repeat(unsigned int milliseconds);
	virtual void on_middle_button_down_repeat(unsigned int milliseconds);

	virtual void on_init();

	virtual void on_main() { };
	virtual void on_draw() { };
};

extern CGlumolGame* CurrentGame;

CGlumolGame *get_game();
void set_game(CGlumolGame *game);

#endif
