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
#include <boost/python.hpp>
#include <ClanLib/display.h>

using namespace boost::python;

void export_enums(); 
void export_color_class(); 
void export_cpoint_class(); 
void export_cpoint__class(); 
void export_csize_class();
void export_crect_class(); 
void export_crect__class(); 
void export_animation_class(); 
void export_glumolscreen_class(); 
void export_glumolfont(); 
void export_glumolsprite_class(); 
void export_glumolzbuffer_class(); 
void export_console_class();
void export_inpututil_class();
void export_glumolgame_class();
void export_glumolthread_class();
	
void add_corigin_enums();
void add_glumol_region();
void export_exceptions();
void export_component();
void export_filedialog();
void export_stylemanager();
void export_checkbox();
void export_tree_node();
void export_treeview();
void export_button();
void export_window();
void export_gui_manager();
void export_resource_manager();
void export_frame();
void export_input_box();
void export_scroll_bar();
void export_progress_bar();
void export_message_box();
void export_radio_button();
void export_radio_group();
void export_listbox();
void export_label();
void export_input_dialog();
void export_tree_item();
void export_menu();
void export_menu_item();
void export_menu_node();
void export_image();
void export_pathfinder();

void export_sound_buffer();
void export_sound_filter();
void export_sound_buffer_session();
void export_sound_output();
void export_sound_provider();

BOOST_PYTHON_MODULE(poujol)
{
    export_exceptions();
	export_enums(); 
	export_color_class(); 
	export_cpoint_class(); 
	export_cpoint__class(); 
	export_csize_class();
	export_crect_class(); 
	export_crect__class(); 
	export_glumolfont();
	export_animation_class(); 
	export_glumolscreen_class(); 
	export_glumolsprite_class();
	export_glumolzbuffer_class();
	export_console_class();
	export_inpututil_class();
	export_glumolgame_class();
    export_glumolthread_class();
        
	add_glumol_region();
	export_component();
	export_gui_manager();
	export_window();
	export_button();
	export_filedialog();
	export_stylemanager();
	// export_checkbox();
	export_tree_node();
	export_treeview();
	export_resource_manager();
	export_frame();
	export_scroll_bar();
	export_progress_bar();
	export_message_box();
	export_radio_button();
	export_radio_group();
	export_listbox();
	export_label();
	export_input_dialog();
	export_input_box();
	export_tree_item();
	export_menu();
	export_menu_item();
	export_menu_node();
	export_image();
	export_pathfinder();

	export_sound_buffer();
	export_sound_filter();
	export_sound_buffer_session();
	export_sound_output();
	export_sound_provider();
}
