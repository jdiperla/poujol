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

template<class T>
struct CGlumolSprite_wrap : public T
{
	CGlumolSprite_wrap(PyObject* self_, boost::shared_ptr<CGlumolSprite_wrap<CGlumolSprite> > parent)
		: T(parent.get()),  self(self_) {}

	CGlumolSprite_wrap(PyObject* self_, CGlumolScreen *parent)
		: T(parent),  self(self_) {}

    /*
    CGlumolSprite_wrap(PyObject* self_, T const& rhs)
                 : T(rhs), self(self_) {}

	CGlumolSprite_wrap(PyObject* self_,SHARED_PTR(CGlumolScreen) parent, int cx, int cy, CL_Color color)
		: T(parent, cx, cy, color),  self(self_) {}

    CGlumolSprite_wrap(PyObject* self_, SHARED_PTR(T) rhs, int cx, int cy, CL_Color color)
                : T(rhs, cx, cy, color), self(self_) {}
    */
	
    void on_key_down(int Key){ call_method<void>(self, "on_key_down", Key); }
	void on_key_up(int Key){ call_method<void>(self, "on_key_up", Key); }
	void on_key_down_repeat(int	Key, unsigned int Milliseconds){ call_method<void>(self, "on_key_down_repeat", Key, Milliseconds); }
	void on_mouse_button_down(unsigned int button){ call_method<void>(self, "on_mouse_button_down", button); }
	void on_mouse_button_up(unsigned  button){ call_method<void>(self, "on_mouse_button_up", button); }
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
	void on_before_draw(){ call_method<void>(self, "on_before_draw"); }
	void on_after_draw(){ call_method<void>(self, "on_after_draw"); }
	void on_focus(SHARED_PTR(T) sprite) { call_method<void>(self, "on_focus", sprite); }
	void on_lose_focus(SHARED_PTR(T) sprite) { call_method<void>(self, "on_lose_focus", sprite); }
	void on_frame_changed(int NewFrame){ call_method<void>(self, "on_frame_changed", NewFrame); }
	void on_animation_finished(){ call_method<void>(self, "on_animation_finished"); }

	void default_on_key_down(int Key){ T::on_key_down(Key); }
	void default_on_key_up(int Key){ T::on_key_up(Key); }
	void default_on_key_down_repeat(int Key, unsigned int Milliseconds){ T::on_key_down_repeat(Key, Milliseconds); } 
	void default_on_mouse_button_down(unsigned int button){ T::on_mouse_button_down(button); } 
	void default_on_mouse_button_up(unsigned button){ T::on_mouse_button_up(button); } 
	void default_on_left_button_down(){ T::on_left_button_down(); } 
	void default_on_right_button_down(){ T::on_right_button_down(); } 
	void default_on_left_button_up(){ T::on_left_button_up(); } 
	void default_on_right_button_up(){ T::on_right_button_up(); } 
	void default_on_middle_button_down(){ T::on_middle_button_down(); } 
	void default_on_middle_button_up(){ T::on_middle_button_up(); } 
	void default_on_mouse_wheel_up(){ T::on_mouse_wheel_up(); }
	void default_on_mouse_wheel_down(){ T::on_mouse_wheel_down(); } 
	void default_on_mouse_move(CL_Pointf d, CL_Pointf e, float axis){ T::on_mouse_move(d,e,axis); } 
	void default_on_left_button_down_repeat(unsigned int Miliseconds){ T::on_left_button_down_repeat(Miliseconds); } 
	void default_on_right_button_down_repeat(unsigned Miliseconds){ T::on_right_button_down_repeat(Miliseconds); } 
	void default_on_middle_button_down_repeat(unsigned Miliseconds){ T::on_middle_button_down_repeat(Miliseconds); } 
	void default_on_before_draw(){ T::on_before_draw(); }
	void default_on_after_draw(){ T::on_after_draw(); }
	void default_on_focus(SHARED_PTR(T) sprite){ T::on_focus(sprite);  }
	void default_on_lose_focus(SHARED_PTR(T) sprite){T::on_lose_focus(sprite);  }
	void default_on_frame_changed(int NewFrame){ T::on_frame_changed(NewFrame); }
	void default_on_animation_finished(){ T::on_animation_finished();  }

	void set_couleur(object obj) {
        if(PyObject_IsInstance(obj.ptr(), (PyObject*) &PyList_Type)) {
            int r, g, b, a = 255;
            r = extract<int>(obj[0]);
            g = extract<int>(obj[1]);
            b = extract<int>(obj[2]);
            if(PyList_Size(obj.ptr()) >= 4)
                a = extract<int>(obj[3]);
            T::set_color(CL_Color(r, g, b, a));
        }
        else if(PyObject_IsInstance(obj.ptr(), (PyObject*) &PyTuple_Type)) {
            int r, g, b, a = 255;
            r = extract<int>(obj[0]);
            g = extract<int>(obj[1]);
            b = extract<int>(obj[2]);
            if(PyTuple_Size(obj.ptr()) >= 4)
                a = extract<int>(obj[3]);
            T::set_color(CL_Color(r, g, b, a));
        }
        else {
            CL_Color c = extract<CL_Color>(obj);
            T::set_color(c);
        }
	}

	CL_Color get_couleur() {
		return T::get_color();
	}
	
    object Get_Parent3()
    {
        CGlumolSprite_wrap *p = (CGlumolSprite_wrap*) this->parent.px;
        if(p) {
        	return object(borrowed(p->self));
        }
        else {
        	return object();
        }
    }

    void Set_Parent3(const boost::shared_ptr<CGlumolSprite_wrap> &p) {
        this->parent = p;
    }

	tuple get_scale() {
		return make_tuple(this->Scale_x, this->Scale_y);
	}

	void set_scale(object obj) {
		float factor_x = this->Scale_x;
		float factor_y = this->Scale_y;
		if(PyNumber_Check(obj.ptr())) {
			factor_x = factor_y = extract<float>(obj);
		}
		else if(PyObject_IsInstance(obj.ptr(), (PyObject*) &PyList_Type) ||
				PyObject_IsInstance(obj.ptr(), (PyObject*) &PyTuple_Type)) {
			factor_x = extract<float>(obj[0]);
			factor_y = extract<float>(obj[1]);
		}
		etirer(factor_x / this->Scale_x, factor_y / this->Scale_y);
	}

	tuple get_size() {
		return make_tuple(this->size.width, this->size.height);
	}

	void set_size(object obj) {
		float width = this->size.width;
		float height = this->size.height;
		if(PyObject_IsInstance(obj.ptr(), (PyObject*) &PyList_Type) ||
				PyObject_IsInstance(obj.ptr(), (PyObject*) &PyTuple_Type)) {
			width = extract<float>(obj[0]);
			height = extract<float>(obj[1]);
    		this->size = CL_Sizef(width, height);
		}
        else {
            this->size = extract<CL_Sizef>(obj);
        }
	}

    void Set_parent(SHARED_PTR(CGlumolSprite_wrap) &sprite)
	{
		this->parent = sprite;
	}

    CL_Pointf const& get_position() {
		return this->Position;
	}

    void set_position(object obj) {
            if(PyObject_IsInstance(obj.ptr(), (PyObject*) &PyList_Type) ||
               PyObject_IsInstance(obj.ptr(), (PyObject*) &PyTuple_Type)) {
                float x = extract<float>(obj[0]);
                float y = extract<float>(obj[1]);
                T::set_position(CL_Pointf(x, y));
            }
            else {
                CL_Pointf p = extract<CL_Pointf>(obj);
                T::set_position(p);
            }
	}
	
	Regions &get_regions() { return this->regions; }

	PyObject* self;
};
