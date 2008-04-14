
#include "stdafx.h"
#include <boost/python.hpp>
#include <ClanLib/gui.h>
#include <ClanLib/core.h>

using namespace boost::python;

struct CL_Component_wrap : public CL_Component
{
	CL_Component_wrap(PyObject* self_, CL_Component* parent, CL_StyleManager *style = NULL)
		: CL_Component(parent, style), self(self_)
	{
		init_slots();
	}

	CL_Component_wrap(PyObject* self_, const CL_Rect &rect, CL_Component* parent, CL_StyleManager *style = NULL)
		: CL_Component(rect, parent, style), self(self_)
	{
		init_slots();
	}

	void init_slots()
	{
		slot_paint = sig_paint().connect(this, &CL_Component_wrap::on_paint);
		slot_paint_children = sig_paint_children().connect(this, &CL_Component_wrap::on_paint_children);
		slot_begin_paint = sig_begin_paint().connect(this, &CL_Component_wrap::on_begin_paint);
		slot_end_paint = sig_end_paint().connect(this, &CL_Component_wrap::on_end_paint);
		slot_mouse_down = sig_mouse_down().connect(this, &CL_Component_wrap::on_mouse_down);
		slot_mouse_up = sig_mouse_up().connect(this, &CL_Component_wrap::on_mouse_up);
		slot_mouse_enter = sig_mouse_enter().connect(this, &CL_Component_wrap::on_mouse_enter);
		slot_mouse_leave = sig_mouse_leave().connect(this, &CL_Component_wrap::on_mouse_leave);
		slot_got_focus = sig_got_focus().connect(this, &CL_Component_wrap::on_got_focus);
		slot_lost_focus = sig_lost_focus().connect(this, &CL_Component_wrap::on_lose_focus);
		slot_key_down = sig_key_down().connect(this, &CL_Component_wrap::on_key_down);
		slot_key_up = sig_key_up().connect(this, &CL_Component_wrap::on_key_up);
		slot_set_options = sig_set_options().connect(this, &CL_Component_wrap::on_set_options);
		slot_close = sig_close().connect(this, &CL_Component_wrap::on_close);
		slot_resize = sig_resize().connect(this, &CL_Component_wrap::on_resize);
		slot_move = sig_move().connect(this, &CL_Component_wrap::on_move);
		slot_child_before_add = sig_child_before_add().connect(this, &CL_Component_wrap::on_child_before_add);
		slot_child_before_remove = sig_child_before_remove().connect(this, &CL_Component_wrap::on_child_before_remove);
		slot_child_add = sig_child_add().connect(this, &CL_Component_wrap::on_child_add);
		slot_child_remove = sig_child_remove().connect(this, &CL_Component_wrap::on_child_remove);
		slot_transform_coords = sig_transform_coords().connect(this, &CL_Component_wrap::on_transform_coords);
		slot_get_minimum_size = sig_get_minimum_size().connect(this, &CL_Component_wrap::on_get_minimum_size);
		slot_get_maximum_size = sig_get_maximum_size().connect(this, &CL_Component_wrap::on_get_maximum_size);
		slot_get_preferred_size = sig_get_preferred_size().connect(this, &CL_Component_wrap::on_get_preferred_size);
	}

	void on_paint()
	{
		call_method<void>(self, "on_paint");
	}

	void default_on_paint()
	{
	}

	void on_paint_children()
	{
		call_method<void>(self, "on_paint_children");
	}

	void default_on_paint_children()
	{
	}

	void on_begin_paint()
	{
		call_method<void>(self, "on_begin_paint");
	}

	void default_on_begin_paint()
	{
	}

	void on_end_paint()
	{
		call_method<void>(self, "on_end_paint");
	}

	void default_on_end_paint()
	{
	}

	void on_mouse_down(const CL_InputEvent& event)
	{
		call_method<void>(self, "on_mouse_down", event);
	}

	void default_on_mouse_down(const CL_InputEvent&)
	{
	}

	void on_mouse_up(const CL_InputEvent& event)
	{
		call_method<void>(self, "on_mouse_up", event);
	}

	void default_on_mouse_up(const CL_InputEvent&)
	{
	}

	void on_mouse_enter()
	{
		call_method<void>(self, "on_mouse_enter");
	}

	void default_on_mouse_enter()
	{
	}

	void on_mouse_leave()
	{
		call_method<void>(self, "on_mouse_leave");
	}

	void default_on_mouse_leave()
	{
	}

	void on_key_down(const CL_InputEvent& event)
	{
		call_method<void>(self, "on_key_down", event);
	}

	void default_on_key_down(const CL_InputEvent&)
	{
	}

	void on_key_up(const CL_InputEvent& event)
	{
		call_method<void>(self, "on_key_up", event);
	}

	void default_on_key_up(const CL_InputEvent&)
	{
	}

	void on_set_options(const CL_DomElement& elem)
	{
		call_method<void>(self, "on_set_options", elem);
	}

	void default_on_set_options(const CL_DomElement&)
	{
	}

	void on_got_focus()
	{
		call_method<void>(self, "on_got_focus");
	}

	void default_on_got_focus()
	{
	}

	void on_lose_focus()
	{
		call_method<void>(self, "on_lose_focus");
	}

	void default_on_lose_focus()
	{
	}

	void on_close()
	{
		call_method<void>(self, "on_close");
	}

	void default_on_close()
	{
	}

	void on_move(int x, int y)
	{
		call_method<void>(self, "on_move", x, y);
	}

	void default_on_move(int x, int y)
	{
	}

	void on_resize(int x, int y)
	{
		call_method<void>(self, "on_resize", x, y);
	}

	void default_on_resize(int x, int y)
	{
	}

	void on_child_before_add(CL_Component* component)
	{
		call_method<void>(self, "on_child_before_add", component);
	}

	void default_on_child_before_add(CL_Component* component)
	{
	}

	void on_child_add(CL_Component* component)
	{
		call_method<void>(self, "on_child_add", component);
	}

	void default_on_child_add(CL_Component* component)
	{
	}

	void on_child_before_remove(CL_Component* component)
	{
		call_method<void>(self, "on_child_before_remove", component);
	}

	void default_on_child_before_remove(CL_Component* component)
	{
	}

	void on_child_remove(CL_Component* component)
	{
		call_method<void>(self, "on_child_remove", component);
	}

	void default_on_child_remove(CL_Component* component)
	{
	}

	void on_transform_coords(int& x, int& y)
	{
		call_method<void>(self, "on_transform_coords", x, y);
	}

	void default_on_transform_coords(int&, int&)
	{
	}

	void on_get_minimum_size(CL_Size &size)
	{
		call_method<void>(self, "on_get_minimum_size", size);
	}

	void default_on_get_minimum_size(CL_Size &size)
	{
	}

	void on_get_maximum_size(CL_Size &size)
	{
		call_method<void>(self, "on_get_maximum_size", size);
	}

	void default_on_get_maximum_size(CL_Size &size)
	{
	}

	void on_get_preferred_size(CL_Size &size)
	{
		call_method<void>(self, "on_get_preferred_size", size);
	}

	void default_on_get_preferred_size(CL_Size &size)
	{
	}

	CL_Slot slot_paint,
		slot_paint_children,
		slot_begin_paint,
		slot_end_paint,
		slot_mouse_down,
		slot_mouse_up,
		slot_key_down,
		slot_key_up,
		slot_set_options,
		slot_mouse_enter,
		slot_mouse_leave,
		slot_got_focus,
		slot_lost_focus,
		slot_close,
		slot_resize,
		slot_move,
		slot_child_before_add,
		slot_child_before_remove,
		slot_child_add,
		slot_child_remove,
		slot_transform_coords,
		slot_get_minimum_size,
		slot_get_maximum_size,
		slot_get_preferred_size;

	PyObject* self;
};

template<class T>
struct CL_Component_wrap2
{
	CL_Component_wrap2(T *t)
	{
		t->init_slots();
//		slot_paint = t->sig_paint().connect(t, &CL_Component_wrap::on_paint);
/*		slot_paint_children = t->sig_paint_children().connect(t, &CL_Component_wrap::on_paint_children);
		slot_begin_paint = t->sig_begin_paint().connect(t, &CL_Component_wrap::on_begin_paint);
		slot_end_paint = t->sig_end_paint().connect(t, &CL_Component_wrap::on_end_paint);
		slot_mouse_down = t->sig_mouse_down().connect(t, &CL_Component_wrap::on_mouse_down);
		slot_mouse_up = t->sig_mouse_up().connect(t, &CL_Component_wrap::on_mouse_up);
		slot_mouse_enter = t->sig_mouse_enter().connect(t, &CL_Component_wrap::on_mouse_enter);
		slot_mouse_leave = t->sig_mouse_leave().connect(t, &CL_Component_wrap::on_mouse_leave);
		slot_got_focus = t->sig_got_focus().connect(t, &CL_Component_wrap::on_got_focus);
		slot_lost_focus = t->sig_lost_focus().connect(t, &CL_Component_wrap::on_lose_focus);
		slot_key_down = t->sig_key_down().connect(t, &CL_Component_wrap::on_key_down);
		slot_key_up = t->sig_key_up().connect(t, &CL_Component_wrap::on_key_up);
		slot_set_options = t->sig_set_options().connect(t, &CL_Component_wrap::on_set_options);
		slot_close = t->sig_close().connect(t, &CL_Component_wrap::on_close);
		slot_resize = t->sig_resize().connect(t, &CL_Component_wrap::on_resize);
		slot_move = t->sig_move().connect(t, &CL_Component_wrap::on_move);
		slot_child_before_add = t->sig_child_before_add().connect(t, &CL_Component_wrap::on_child_before_add);
		slot_child_before_remove = t->sig_child_before_remove().connect(t, &CL_Component_wrap::on_child_before_remove);
		slot_child_add = t->sig_child_add().connect(t, &CL_Component_wrap::on_child_add);
		slot_child_remove = t->sig_child_remove().connect(t, &CL_Component_wrap::on_child_remove);
		slot_transform_coords = t->sig_transform_coords().connect(t, &CL_Component_wrap::on_transform_coords);
		slot_get_minimum_size = t->sig_get_minimum_size().connect(t, &CL_Component_wrap::on_get_minimum_size);
		slot_get_maximum_size = t->sig_get_maximum_size().connect(t, &CL_Component_wrap::on_get_maximum_size);
		slot_get_preferred_size = t->sig_get_preferred_size().connect(t, &CL_Component_wrap::on_get_preferred_size);
*/	}

	CL_Slot slot_paint,
		slot_paint_children,
		slot_begin_paint,
		slot_end_paint,
		slot_mouse_down,
		slot_mouse_up,
		slot_key_down,
		slot_key_up,
		slot_set_options,
		slot_mouse_enter,
		slot_mouse_leave,
		slot_got_focus,
		slot_lost_focus,
		slot_close,
		slot_resize,
		slot_move,
		slot_child_before_add,
		slot_child_before_remove,
		slot_child_add,
		slot_child_remove,
		slot_transform_coords,
		slot_get_minimum_size,
		slot_get_maximum_size,
		slot_get_preferred_size;
};

struct CL_TreeView_wrap : public CL_TreeView, public CL_Component_wrap2<CL_TreeView_wrap>
{
	CL_TreeView_wrap(PyObject* self_, CL_Component* component, CL_StyleManager* style = NULL) :
		CL_TreeView(component, style), CL_Component_wrap2<CL_TreeView_wrap>(this),
                self(self_)
	{
		init_slots();
	}
	
	CL_TreeView_wrap(PyObject* self_, const CL_Rect &rect, CL_Component* component, CL_StyleManager* style = NULL) :
		CL_TreeView(rect, component, style), CL_Component_wrap2<CL_TreeView_wrap>(this),
		self(self_) { }

	void init_slots()
	{
		CL_Slot s1 = sig_selection_changed().connect(this, &CL_TreeView_wrap::on_selection_changed);
		CL_Slot s2 = sig_item_clicked().connect(this, &CL_TreeView_wrap::on_item_clicked);
		CL_Slot s3 = sig_item_added().connect(this, &CL_TreeView_wrap::on_item_added);
		CL_Slot s4 = sig_item_removed().connect(this, &CL_TreeView_wrap::on_item_removed);
		CL_Slot s5 = sig_item_collapsed().connect(this, &CL_TreeView_wrap::on_item_collapsed);
		CL_Slot s6 = sig_item_expanded().connect(this, &CL_TreeView_wrap::on_item_expanded);
		CL_Slot s7 = sig_column_added().connect(this, &CL_TreeView_wrap::on_column_added);
		CL_Slot s8 = sig_column_removed().connect(this, &CL_TreeView_wrap::on_column_removed);
		CL_Slot s9 = sig_clear().connect(this, &CL_TreeView_wrap::on_clear);
		CL_Slot s10 = sig_paint_node().connect(this, &CL_TreeView_wrap::on_paint_node);
	}

	void on_selection_changed(const CL_TreeNode &node) { call_method<void>(self, "on_selection_changed", boost::ref(node)); }
	void default_on_selection_changed(const CL_TreeNode &node) {}

	void on_item_clicked(const CL_TreeNode &node) { call_method<void>(self, "on_item_clicked", boost::ref(node)); }
	void default_on_item_clicked(const CL_TreeNode&) {}

	void on_item_added(const CL_TreeNode &node) { call_method<void>(self, "on_item_added", boost::ref(node)); }
	void default_on_item_added(const CL_TreeNode &node) {}

	void on_item_removed(const CL_TreeNode &node) { call_method<void>(self, "on_item_removed", boost::ref(node)); }
	void default_on_item_removed(const CL_TreeNode &node) {}

	void on_item_collapsed(const CL_TreeNode &node) { call_method<void>(self, "on_item_collapsed", boost::ref(node)); }
	void default_on_item_collapsed(const CL_TreeNode &node) {}

	void on_item_expanded(const CL_TreeNode &node) { call_method<void>(self, "on_item_expanded", boost::ref(node)); }
	void default_on_item_expanded(const CL_TreeNode &node) {}

	void on_column_added(int i) { call_method<void>(self, "on_column_added", i); }
	void default_on_column_added(int i) {}

	void on_column_removed(int i) { call_method<void>(self, "on_column_removed", i); }
	void default_on_column_removed(int i) {}

	void on_clear() { call_method<void>(self, "on_clear"); }
	void default_on_clear() {}

	void on_paint_node(CL_TreeNode* node, CL_Point &point) { call_method<void>(self, "on_paint_node", boost::ref(node), boost::ref(point)); }
	void default_on_paint_node(CL_TreeNode* node, CL_Point &point) {}

	CL_TreeItem* insert_item3(
		const char* l1, const char* l2 = "",
		const char* l3 = "", const char* l4 = "",
		const char* l5 = "", const char* l6 = "",
		const char* l7 = "", const char* l8 = "")
	{
		return CL_TreeNode::insert_item(l1, l2, l3, l4, l5,
			l6, l7, l8);
	}

	CL_TreeNode* insert_item4(
		CL_Component* label1, CL_Component* label2 = NULL,
		CL_Component* label3 = NULL,
		CL_Component* label4 = NULL,
		CL_Component* label5 = NULL,
		CL_Component* label6 = NULL,
		CL_Component* label7 = NULL,
		CL_Component* label8 = NULL)
	{
		return CL_TreeNode::insert_item(label1, label2, label3, label4, label5,
			label6, label8, label8);
	}
	PyObject* self;
};

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(
	insert_item_overloads, insert_item3, 1, 8)

void export_treeview()
{
	class_<CL_TreeView, CL_TreeView_wrap, bases<CL_Component, CL_TreeNode>, boost::noncopyable>("TreeView", init< CL_Component*, optional<CL_StyleManager*> >())
		.def(init<const CL_Rect&, CL_Component*, optional<CL_StyleManager*> >())
		.def("get_column_count", &CL_TreeView::get_column_count)
		.def("get_column_width", &CL_TreeView::get_column_width)
		.def("get_column_name", &CL_TreeView::get_column_name, return_value_policy<copy_const_reference>())
		.def("is_root_decoration_visible", &CL_TreeView::is_root_decoration_visible)
		.def("is_header_visible", &CL_TreeView::is_header_visible)
		.def("add_column", &CL_TreeView::add_column)
		.def("show_root_decoration", &CL_TreeView::show_root_decoration)
		.def("show_header", &CL_TreeView::show_header)
		.def("insert_item3", &CL_TreeView_wrap::insert_item3,
			insert_item_overloads("insert_item3")[return_value_policy<reference_existing_object>()])
	;
}
