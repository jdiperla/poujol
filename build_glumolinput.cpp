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

/*
#include "stdafx.h"
#include <ClanLib/core.h>
#include "GlumolInput.h"
#include <boost/python.hpp>
using namespace boost::python;

struct CGlumolInput_wrap : public CGlumolInput
{
    CGlumolInput_wrap(PyObject* self_)
          : self(self_) {}

   CGlumolInput_wrap(PyObject* self_, CGlumolInput const& rhs)
                 : CGlumolInput(rhs), self(self_) {}

	void OnKeyDown(unsigned int Key){ call_method<void>(self, "OnKeyDown", Key); }
	void OnKeyUp(unsigned int Key){ call_method<void>(self, "OnKeyUp", Key); }
	void OnKeyDownRepeat(unsigned int Key, unsigned int Milliseconds){ call_method<void>(self, "OnKeyDownRepeat", Key, Milliseconds); }
	void OnMouseButtonDown(unsigned int button){ call_method<void>(self, "OnMouseButtonDown", button); }
	void OnMouseButtonUp(unsigned int button){ call_method<void>(self, "OnMouseButtonUp", button); }
	void OnLeftButtonDown(){ call_method<void>(self, "OnLeftButtonDown"); }
	void OnRightButtonDown(){ call_method<void>(self, "OnRightButtonDown"); }
	void OnLeftButtonUp(){ call_method<void>(self, "OnLeftButtonUp"); }
	void OnRightButtonUp(){ call_method<void>(self, "OnRightButtonUp"); }
	void OnMiddleButtonDown(){ call_method<void>(self, "OnMiddleButtonDown"); }
	void OnMiddleButtonUp(){ call_method<void>(self, "OnMiddleButtonUp"); }
	void OnMouseWheelUp(){ call_method<void>(self, "OnMouseWheelUp"); }
	void OnMouseWheelDown(){ call_method<void>(self, "OnMouseWheelDown"); }
	void OnMouseMove(CL_Point o,CL_Point n ,float axis){ call_method<void>(self, "OnMouseMove", o,n,axis); }
	
	void OnLeftButtonDownRepeat(unsigned int Milliseconds){ call_method<void>(self, "OnLeftButtonDownRepeat", Milliseconds); }

	void OnRightButtonDownRepeat(unsigned int Miliseconds){ call_method<void>(self, "OnRightButtonDownRepeat", Miliseconds); }
	void OnMiddleButtonDownRepeat(unsigned int Miliseconds){ call_method<void>(self, "OnMiddleButtonDownRepeat", Miliseconds); }

	void default_OnKeyDown(unsigned int Key){ CGlumolInput::OnKeyDown(Key); }
	void default_OnKeyUp(unsigned int Key){ CGlumolInput::OnKeyUp(Key); }
	void default_OnKeyDownRepeat(unsigned int Key, unsigned int Milliseconds){ CGlumolInput::OnKeyDownRepeat(Key, Milliseconds); } 
	void default_OnMouseButtonDown(unsigned int button){ CGlumolInput::OnMouseButtonDown(button); } 
	void default_OnMouseButtonUp(unsigned int button){ CGlumolInput::OnMouseButtonUp(button); } 
	void default_OnLeftButtonDown(){ CGlumolInput::OnLeftButtonDown(); } 
	void default_OnRightButtonDown(){ CGlumolInput::OnRightButtonDown(); } 
	void default_OnLeftButtonUp(){ CGlumolInput::OnLeftButtonUp(); } 
	void default_OnRightButtonUp(){ CGlumolInput::OnRightButtonUp(); } 
	void default_OnMiddleButtonDown(){ CGlumolInput::OnMiddleButtonDown(); } 
	void default_OnMiddleButtonUp(){ CGlumolInput::OnMiddleButtonUp(); } 
	void default_OnMouseWheelUp(){ CGlumolInput::OnMouseWheelUp(); }
	void default_OnMouseWheelDown(){ CGlumolInput::OnMouseWheelDown(); } 
	void default_OnMouseMove(CL_Point d, CL_Point e, float axis){ CGlumolInput::OnMouseMove(d,e,axis); } 
	void default_OnLeftButtonDownRepeat(unsigned int Miliseconds){ CGlumolInput::OnLeftButtonDownRepeat(Miliseconds); } 
	void default_OnRightButtonDownRepeat(unsigned int Miliseconds){ CGlumolInput::OnRightButtonDownRepeat(Miliseconds); } 
	void default_OnMiddleButtonDownRepeat(unsigned int Miliseconds){ CGlumolInput::OnMiddleButtonDownRepeat(Miliseconds); } 

     PyObject* self;
};

void add_glumolinput_class()
{
/*	class_<CGlumolInput, CGlumolInput_wrap>("GlumolInput")
		
	 .def("OnKeyDown",&CGlumolInput::OnKeyDown , &CGlumolInput_wrap::default_OnKeyDown)
	 .def("OnKeyUp",&CGlumolInput::OnKeyUp , &CGlumolInput_wrap::default_OnKeyUp)
     .def("OnKeyDownRepeat",&CGlumolInput::OnKeyDownRepeat , &CGlumolInput_wrap::default_OnKeyDownRepeat)
     .def("OnMouseButtonDown", &CGlumolInput::OnMouseButtonDown, &CGlumolInput_wrap::default_OnMouseButtonDown)
     .def("OnMouseButtonUp", &CGlumolInput::OnMouseButtonUp, &CGlumolInput_wrap::default_OnMouseButtonUp)
     .def("OnLeftButtonDown", &CGlumolInput::OnLeftButtonDown, &CGlumolInput_wrap::default_OnLeftButtonDown)
     .def("OnRightButtonDown", &CGlumolInput::OnRightButtonDown, &CGlumolInput_wrap::default_OnRightButtonDown)
     .def("OnLeftButtonUp", &CGlumolInput::OnLeftButtonUp, &CGlumolInput_wrap::default_OnLeftButtonUp)
     .def("OnRightButtonUp", &CGlumolInput::OnRightButtonUp, &CGlumolInput_wrap::default_OnRightButtonUp)
     .def("OnMiddleButtonDown", &CGlumolInput::OnMiddleButtonDown, &CGlumolInput_wrap::default_OnMiddleButtonDown)
     .def("OnMiddleButtonUp",&CGlumolInput::OnMiddleButtonUp , &CGlumolInput_wrap::default_OnMiddleButtonUp)
     .def("OnMouseWheelUp",&CGlumolInput::OnMouseWheelUp , &CGlumolInput_wrap::default_OnMouseWheelUp)
     .def("OnMouseWheelDown", &CGlumolInput::OnMouseWheelDown, &CGlumolInput_wrap::default_OnMouseWheelDown)
     .def("OnMouseMove",&CGlumolInput::OnMouseMove , &CGlumolInput_wrap::default_OnMouseMove)
     .def("OnLeftButtonDownRepeat", &CGlumolInput::OnLeftButtonDownRepeat, &CGlumolInput_wrap::default_OnLeftButtonDownRepeat)
     .def("OnRightButtonDownRepeat", &CGlumolInput::OnRightButtonDownRepeat, &CGlumolInput_wrap::default_OnRightButtonDownRepeat)
     .def("OnMiddleButtonDownRepeat",&CGlumolInput::OnMiddleButtonDownRepeat , &CGlumolInput_wrap::default_OnMiddleButtonDownRepeat)
	 ; 
}


*/