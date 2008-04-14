/*
Glumol, an adventure game creator.
Copyright (C) 2002 Alexis Contour & Sylvain Baubeau
*/

#include "stdafx.h"

#undef _POSIX_C_SOURCE
#include <boost/python.hpp>

#include "glumolgame.h"

#ifndef WIN32
 #include <libintl.h>
 #define _(String) gettext (String)
#else
#define _(String) String
#endif

using namespace boost::python;

void clean_up(void)
{
	if(CurrentGame)
		delete CurrentGame;
}

struct CGlumolGame_wrap : CGlumolGame
{
    CGlumolGame_wrap(PyObject* self_, bool _fullscreen = false,
                int width = DEFAULT_SCREEN_WIDTH,
                int height = DEFAULT_SCREEN_HEIGHT,
                int depth = DEFAULT_SCREEN_DEPTH,
                const gstring& title = DEFAULT_SCREEN_TITLE,
                bool use_sdl = false,
                bool allow_resize = false) :
        CGlumolGame(_fullscreen, width, height, depth,
                    title, use_sdl, allow_resize),
        self(self_)
    {}

    CGlumolGame_wrap(PyObject* self_, CGlumolGame const& rhs)
        : CGlumolGame(rhs), self(self_) {}
    
    void on_resize(int width, int height) {  call_method<void>(self, "on_resize", width, height); }
    void on_close() {  call_method<void>(self, "on_close"); }
    
    void on_main() {  call_method<void>(self, "on_main"); }
    void on_draw() {  call_method<void>(self, "on_draw"); }

    void on_key_down(int Key){ call_method<void>(self, "on_key_down", Key); }
    void on_key_up(int Key){ call_method<void>(self, "on_key_up", Key); }
    void on_key_down_repeat(int Key, unsigned int Milliseconds){ call_method<void>(self, "on_key_down_repeat", Key, Milliseconds); }
    void on_mouse_button_down(unsigned int button){ call_method<void>(self, "on_mouse_button_down", button); }    
    void on_mouse_button_up(unsigned int button){ call_method<void>(self, "on_mouse_button_up", button); }
    void on_left_button_down(){ call_method<void>(self, "on_left_button_down"); }
    void on_right_button_down(){ call_method<void>(self, "on_right_button_down"); }
    void on_left_button_up(){ call_method<void>(self, "on_left_button_up"); }
    void on_right_button_up(){ call_method<void>(self, "on_right_button_up"); }
    void on_middle_button_down(){ call_method<void>(self, "on_middle_button_down"); }
    void on_middle_button_up(){ call_method<void>(self, "on_middle_button_up"); }
    void on_mouse_wheel_up(){ call_method<void>(self, "on_mouse_wheel_up"); }
    void on_mouse_wheel_down(){ call_method<void>(self, "on_mouse_wheel_down"); }
    void on_mouse_move(CL_Pointf o,CL_Pointf n ,float axis){ call_method<void>(self, "on_mouse_move", o,n,axis); }
    
    void on_left_button_down_repeat(unsigned int Milliseconds){ call_method<void>(self, "on_left_button_down_repeat", Milliseconds); }
    void on_right_button_down_repeat(unsigned int Miliseconds){ call_method<void>(self, "on_right_button_down_repeat", Miliseconds); }
    void on_middle_button_down_repeat(unsigned int Miliseconds){ call_method<void>(self, "on_middle_button_down_repeat", Miliseconds); }

    void default_on_key_down(int Key){ CGlumolGame::on_key_down(Key); }
    void default_on_key_up(int Key){ CGlumolGame::on_key_up(Key); }
    void default_on_key_down_repeat(int Key, unsigned int Milliseconds){ CGlumolGame::on_key_down_repeat(Key, Milliseconds); } 
    void default_on_mouse_button_down(unsigned int button){ CGlumolGame::on_mouse_button_down(button); } 
    void default_on_mouse_button_up(unsigned int button){ CGlumolGame::on_mouse_button_up(button); } 
    void default_on_left_button_down(){ CGlumolGame::on_left_button_down(); } 
    void default_on_right_button_down(){ CGlumolGame::on_right_button_down(); } 
    void default_on_left_button_up(){ CGlumolGame::on_left_button_up(); } 
    void default_on_right_button_up(){ CGlumolGame::on_right_button_up(); } 
    void default_on_middle_button_down(){ CGlumolGame::on_middle_button_down(); } 
    void default_on_middle_button_up(){ CGlumolGame::on_middle_button_up(); } 
    void default_on_mouse_wheel_up(){ CGlumolGame::on_mouse_wheel_up(); }
    void default_on_mouse_wheel_down(){ CGlumolGame::on_mouse_wheel_down(); } 
    void default_on_mouse_move(CL_Pointf d, CL_Pointf e, float axis){ CGlumolGame::on_mouse_move(d,e,axis); } 
    void default_on_left_button_down_repeat(unsigned int Miliseconds){ CGlumolGame::on_left_button_down_repeat(Miliseconds); } 
    void default_on_right_button_down_repeat(unsigned int Miliseconds){ CGlumolGame::on_right_button_down_repeat(Miliseconds); } 
    void default_on_middle_button_down_repeat(unsigned int Miliseconds){ CGlumolGame::on_middle_button_down_repeat(Miliseconds); } 

    void default_on_resize(int width, int height){ CGlumolGame::on_resize(width, height); }
    void default_on_close(){ CGlumolGame::on_close(); }

    void default_on_main(){ CGlumolGame::on_main() ;}
    void default_on_draw(){ CGlumolGame::on_draw() ;}

    PyObject* self;
};

void export_inpututil_class()
{
}

struct glumolgame_pickle_suite : boost::python::pickle_suite
{
    static
    boost::python::tuple
    getinitargs(CGlumolGame const& w)
    {
        return boost::python::make_tuple();
    }

    static
    boost::python::tuple
    getstate(boost::python::object w_obj)
    {
        using namespace boost::python;
        CGlumolGame& w = extract<CGlumolGame&>(w_obj)();

        return make_tuple(w_obj.attr("__dict__"), w.get_window_title(), object(w.current_focused_obj));
    }

    static
    void
    setstate(boost::python::object w_obj, boost::python::tuple state)
    {
        using namespace boost::python;
        CGlumolGame& w = extract<CGlumolGame&>(w_obj)();
        
        if (boost::python::len(state) != 3)
        {
          PyErr_SetObject(PyExc_ValueError,
                          ("expected 1-item tuple in call to __setstate__; got %s"
                           % state).ptr()
              );
          throw_error_already_set();
        }
        
        // restore the object's __dict__
        dict d = extract<dict>(w_obj.attr("__dict__"))();
        d.update(state[0]);
        
        w.set_window_title(extract<std::string>(state[1]));
            
        object o = extract<object>(state[2]);
    }
        
    static bool getstate_manages_dict() { return true; }
};

void export_glumolgame_class()
{
    const char *game_doc = _(
            "The main object of your game. It's used to "
            "create the game, it's window, load the resources, and "
            "owns the Screen object. When a game a launch, the 'run' method "
            "is called, which calls the 'on_main' event. This is where "
            "you can place your code if you want it to be executed "
            "at the very beginning of your game.");

    const char *use_sdl_doc = _(
            "By default, Glumol uses 3D rendering (using OpenGL) so that "
            "'modern' video cards accelerates rendering. On old hardware "
            "you may want to use 2D rendering, using the SDL library. For "
            "that, set 'use_sdl' at True.\n Note that scaling and rotating "
            "with then be really slow.");

    const char *width_doc = _(
            "Change this value to set the width of your game's window.\n"
            "Default is 640.");
            
    const char *size_doc = _(
            "The size of the game windows.\n"
            "Default is (640, 480) and most platforms.");

    const char *height_doc = _(
            "Change this value to set the height of your game's window.\n"
            "Default is 480.");
            
    const char *resource_filename_doc = _(
            "This is the file where the datas of your game will be stored.");
    
    const char *fullscreen_doc = _(
            "Setting 'fullscreen' to True will make your game... fullscreen");
            
    const char *framerate_doc = _(
            "The number of frames per second (as a read-only integer value)");
            
    const char *screen_doc = _(
            "Every displayed sprite is a child of this Screen object. ");
            
    const char *name_doc = _(
            "The name of your game. It will be displayed in the title bar "
            "of your game's window.");
            
    const char *position_doc = _(
            "The position of the top left corner of your game's window");
            
    const char *update_doc = _(
            "This method is called to draw the game, take care of the user's "
            "inputs, update the animations, ...\n");    
    
    const char *take_screenshot_doc = _(
            "take_screenshot(filename)\n"
            "Take a screenshot of the screen and save it as 'filename'");    

    const char *run_doc = _(
            "Launch the game. ");
                        
    const char *quit_doc = _(
            "Quit the game, as soon as possible. The play is not saved.");
            
    const char *on_resize_doc = _(
            "Event.\n"
            "This function is called when the window is resized. "
            "The game must have been created with the argument 'resize' at True.");

    const char *on_close_doc = _(
            "Event.\n"
            "This function is called when the user presses "
            "the window close button. The default behaviour is to close the game.");

    const char *on_main_doc = _(
            "Event.\n"
            "This function is called by 'run' during the startup of "
            "your game. This is the entry point of your game.");
            
    const char *on_draw_doc = _(
            "on_draw_doc(self)\n"
            "Event.\n"
            "'on_draw' is called at every screen refresh.");
            
    const char *on_key_down_doc = _(
        "on_key_down(self, key)\n"
        "Event.\n" \
        "Called when a key is pressed on the keyboard. " \
        "The function gets the value of the pressed key in the " \
        "argument 'key'.");
        
    const char *on_key_up_doc = _(
        "on_key_up(self, key)\n"
        "Event.\n" \
        "Called when a key of the keyboard is released. " \
        "The function gets the value of the released key in the " \
        "argument 'key'.");
        
    const char *on_key_down_repeat_doc = _(
        "on_key_down_repeat_(self, key, delay)\n"
        "Event.\n" \
        "Called when a key of the keyboard is holded " \
        "The function gets the code of the pressed key " \
        "in the 'key' argument and the time " \
        "since the key was pressed " \
        "in the 'delay' argument");
        
    const char *on_mouse_button_down_doc = _(
        "on_mouse_button_down_doc(self, position)\n"
        "Event.\n" \
        "Called when one of mouse buttons is pressed.\n" \
        "The function gets the mouse position in the 'position' " \
        "argument as a tuple.\n" \
        "Note that the event corresponding to the pressed button " \
        "will also be called (on_left_button_down for the left " \
        "button, on_left_button_down for the right button, etc...).");
        
    const char *on_mouse_button_up_doc = _(
        "on_mouse_button_up_doc(self, position)\n"
        "Event.\n" \
        "Called when one of mouse buttons is released.\n" \
        "The function gets the mouse position in the 'position' " \
        "argument as a tuple.\n" \
        "The advertissments are the same than the ones " \
        "on_mouse_button_down.");
        
    const char *on_left_button_down_doc = _(
        "on_left_button_down_doc(self, position)\n"
        "Event.\n" \
        "Called when the left button of the mouse is pressed.\n" \
        "The function gets the mouse position in the 'position' " \
        "argument as a tuple.\n");
    
    const char *on_right_button_down_doc = _(
        "on_right_button_down_doc(self, position)\n"
        "Event.\n" \
        "Called when the right button of the mouse is pressed.\n" \
        "The function gets the mouse position in the 'position' " \
        "argument as a tuple.\n");
    
    const char *on_left_button_up_doc = _(
        "on_left_button_up_doc(self, position)\n"
        "Event.\n" \
        "Called when the left button of the mouse is released.\n" \
        "The function gets the mouse position in the 'position' " \
        "argument as a tuple.\n");
    
    const char *on_right_button_up_doc = _(
        "on_right_button_up_doc(self, position)\n"
        "Event.\n" \
        "Called when the right button of the mouse is released.\n" \
        "The function gets the mouse position in the 'position' " \
        "argument as a tuple.\n");
    
    const char *on_mouse_wheel_up_doc = _(
        "on_mouse_wheel_up_doc(self, position)\n"
        "Event.\n" \
        "Called when the left button of the mouse is released.\n" \
        "The function gets the mouse position in the 'position' " \
        "argument as a tuple.\n");
    
    const char *on_mouse_wheel_down_doc = _(
        "on_mouse_wheel_down_doc(self, position)\n"
        "Event.\n" \
        "Called when the left button of the mouse is released.\n" \
        "The function gets the mouse position in the 'position' " \
        "argument as a tuple.\n");
        
    const char *on_mouse_move_doc = _(
        "on_mouse_move_doc(self, position)\n"
        "Event.\n" \
        "Called when the mouse is moved.\n" \
        "The function gets the mouse position in the 'position' " \
        "argument as a tuple.\n");

    const char *on_left_button_down_repeat_doc = _(
        "on_left_button_down_repeat_doc(self, position, delay)\n"
        "Event.\n" \
        "Called when the left button of the mouse is hold pressed.\n" \
        "The function gets the mouse position in the 'position' " \
        "argument and the time since the key was pressed " \
        "in the 'delay' argument");
        
    const char *on_right_button_down_repeat_doc = _(
        "on_right_button_down_repeat_doc(self, position, delay)\n"
        "Event.\n" \
        "Called when the right button of the mouse is hold pressed.\n" \
        "The function gets the mouse position in the 'position' " \
        "argument and the time since the key was pressed " \
        "in the 'delay' argument");
    
    const char *on_middle_button_down_repeat_doc = _(
        "on_middle_button_down_repeat_doc(self, position, delay)\n"
        "Event.\n" \
        "Called when the middle button of the mouse is hold pressed.\n" \
        "The function gets the mouse position in the 'position' " \
        "argument and the time since the key was pressed " \
        "in the 'delay' argument");
    
    const char *on_middle_button_down_doc = _(
        "on_middle_button_down_doc(self, position)\n"
        "Event.\n" \
        "Called when the middle button of the mouse is pressed.\n" \
        "The function gets the mouse position in the 'position' " \
        "argument as a tuple.\n");
    
    const char *on_middle_button_up_doc = _(
        "on_middle_button_up_doc(self, position)\n"
        "Event.\n" \
        "Called when the middle button of the mouse is released.\n" \
        "The function gets the mouse position in the 'position' " \
        "argument as a tuple.\n");

    const char *clean_up_doc = _(
        "Destroy the game, even if references to the game "
        "still exist. You *should* not use this function");
    
	def("clean_up", &clean_up, clean_up_doc);

    class_<CGlumolGame,CGlumolGame_wrap> (
        "Game", init<bool, int, int, int, const gstring&, bool, bool>( (
                    boost::python::arg("fullscreen") = false,
                    boost::python::arg("width") = DEFAULT_SCREEN_WIDTH,
                    boost::python::arg("height") = DEFAULT_SCREEN_HEIGHT,
                    boost::python::arg("depth") = DEFAULT_SCREEN_DEPTH,
                    boost::python::arg("title") = DEFAULT_SCREEN_TITLE,
                    boost::python::arg("use_sdl") = false,
                    boost::python::arg("allow_resize") = false) ) )
    
        /*
		.add_property("resource_filename",
            &CGlumolGame::get_filename,
            &CGlumolGame::set_filename,
            resource_filename_doc)
        */

        .add_property("archive_filename",
            &CGlumolGame::get_archive_filename,
            &CGlumolGame::set_archive_filename)
            
        .def("is_file_in_archive",
            &CGlumolGame::is_file_in_archive)

        .add_property("width",
            &CGlumolGame::get_width,
            width_doc)
        
        .add_property("set_size",
            &CGlumolGame::set_size,
            size_doc)

        .add_property("fullscreen",
            &CGlumolGame::get_fullscreen,
            &CGlumolGame::set_fullscreen,
            fullscreen_doc)
            
        .add_property("framerate",
            &CGlumolGame::get_framerate,
            framerate_doc)
                    
        .add_property("screen",
			make_function(&CGlumolGame::get_screen, 
                return_value_policy<reference_existing_object >()),
            screen_doc)
            
        .def_readonly("use_sdl", &CGlumolGame::use_sdl, use_sdl_doc)

        .add_property("title",
            &CGlumolGame::get_window_title,
            &CGlumolGame::set_window_title,
            name_doc)
    
        .add_property("position",
            &CGlumolGame::get_window_position,
            &CGlumolGame::set_window_position,
            position_doc)
        
        .def("quit",
            &CGlumolGame::quit,
            quit_doc)
        
        // bidouilles

        .add_property("show_console",
			&CGlumolGame::is_console_visible,
            &CGlumolGame::show_console)
        
		.def("get_console",
            &CGlumolGame::get_console,
            return_value_policy<reference_existing_object>() )

        .def("update",
            &CGlumolGame::update,
            update_doc)

        .def("take_screenshot",
            &CGlumolGame::take_screenshot,
            take_screenshot_doc)
            
        .def("run",
            &CGlumolGame::run,
            run_doc)
            
        .def("on_main",
            &CGlumolGame::on_main,
            &CGlumolGame_wrap::default_on_main,
            on_main_doc)
    
        .def("on_draw",
            &CGlumolGame::on_draw,
            &CGlumolGame_wrap::default_on_draw,
            on_draw_doc)

        .def("on_resize",
            &CGlumolGame::on_resize,
            &CGlumolGame_wrap::default_on_resize,
            on_resize_doc)

        .def("on_close",
            &CGlumolGame::on_close,
            &CGlumolGame_wrap::default_on_close,
            on_close_doc)

        .def("on_key_down",
            &CGlumolGame::on_key_down,
            &CGlumolGame_wrap::default_on_key_down,
            on_key_down_doc)
        
        .def("on_key_up",
            &CGlumolGame::on_key_up,
            &CGlumolGame_wrap::default_on_key_up,
            on_key_up_doc)
        
        .def("on_key_down_repeat",
            &CGlumolGame::on_key_down_repeat,
            &CGlumolGame_wrap::default_on_key_down_repeat,
            on_key_down_repeat_doc)
        
        .def("on_mouse_button_down",
            &CGlumolGame::on_mouse_button_down,
            &CGlumolGame_wrap::default_on_mouse_button_down,
            on_mouse_button_down_doc)
        
        .def("on_mouse_button_up",
            &CGlumolGame::on_mouse_button_up,
            &CGlumolGame_wrap::default_on_mouse_button_up,
            on_mouse_button_up_doc)
        
        .def("on_left_button_down",
            &CGlumolGame::on_left_button_down,
            &CGlumolGame_wrap::default_on_left_button_down,
            on_left_button_down_doc)
        
        .def("on_right_button_down",
            &CGlumolGame::on_right_button_down,
            &CGlumolGame_wrap::default_on_right_button_down,
            on_right_button_down_doc)
        
        .def("on_left_button_up",
            &CGlumolGame::on_left_button_up,
            &CGlumolGame_wrap::default_on_left_button_up,
            on_left_button_up_doc)
        
        .def("on_right_button_up",
            &CGlumolGame::on_right_button_up,
            &CGlumolGame_wrap::default_on_right_button_up,
            on_right_button_up_doc)
        
        .def("on_middle_button_down",
            &CGlumolGame::on_middle_button_down,
            &CGlumolGame_wrap::default_on_middle_button_down,
            on_middle_button_down_doc)
        
        .def("on_middle_button_up",
            &CGlumolGame::on_middle_button_up,
            &CGlumolGame_wrap::default_on_middle_button_up,
            on_middle_button_up_doc)
        
        .def("on_mouse_wheel_up",
            &CGlumolGame::on_mouse_wheel_up,
            &CGlumolGame_wrap::default_on_mouse_wheel_up,
            on_mouse_wheel_up_doc)
    
        .def("on_mouse_wheel_down",
            &CGlumolGame::on_mouse_wheel_down,
            &CGlumolGame_wrap::default_on_mouse_wheel_down,
            on_mouse_wheel_down_doc)
            
		.def("on_mouse_move",
            &CGlumolGame::on_mouse_move,
            &CGlumolGame_wrap::default_on_mouse_move,
            on_mouse_move_doc)
            
        .def("on_left_button_down_repeat",
            &CGlumolGame::on_left_button_down_repeat,
            &CGlumolGame_wrap::default_on_left_button_down_repeat,
            on_left_button_down_repeat_doc)
            
        .def("on_right_button_down_repeat",
            &CGlumolGame::on_right_button_down_repeat,
            &CGlumolGame_wrap::default_on_right_button_down_repeat,
            on_right_button_down_repeat_doc)
        
        .def("on_middle_button_down_repeat",
            &CGlumolGame::on_middle_button_down_repeat,
            &CGlumolGame_wrap::default_on_middle_button_down_repeat,
            on_middle_button_down_repeat_doc)
        
        .add_property("mouse_sensibility",
            &CGlumolGame::get_mouse_sensibility,
            &CGlumolGame::set_mouse_sensibility)
        
        .add_property("mouse_position",
            &CGlumolGame::get_mouse_position,
            &CGlumolGame::set_mouse_position)
        
        .add_property("mouse_acceleration",
            &CGlumolGame::get_mouse_acceleration,
            &CGlumolGame::set_mouse_acceleration)
        
        .add_property("enable_mouse",
            &CGlumolGame::get_enable_mouse,
            &CGlumolGame::set_enable_mouse)

        .add_property("mouse_sprite",
            &CGlumolGame::get_mouse_sprite,
            &CGlumolGame::set_mouse_sprite)

        .def("set_mouse_sprite",
            &CGlumolGame::set_mouse_sprite)

		.def_pickle(glumolgame_pickle_suite())
    ;
}
