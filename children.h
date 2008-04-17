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

#ifndef __CHILDREN_H__
#define __CHILDREN_H__

#include <iostream>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/python/object.hpp>
#include <boost/python/extract.hpp>

using namespace boost::python;

template<typename Z, typename C>
class ChildrenZ : public C
{
public:
    typedef Z montype;
        
	static int convert_index(C &c, PyObject* i_)
	{ 
		extract<long> i(i_);
		if (i.check())
		{
			long index = i();
			if (index < 0)
				index += c.size();
			if (index >= long(c.size()) || index < 0)
			{
				PyErr_SetString(PyExc_IndexError, "Index out of range");
				throw_error_already_set();
			}
			return index;
		}
            
		PyErr_SetString(PyExc_TypeError, "Invalid index type");
		throw_error_already_set();
		return -1;
	}

	static object get_item(C &c, PyObject *i_) {
		if (PySlice_Check(i_)) {
			PySliceObject *slice = (PySliceObject*) i_;
			int from, to;
			if (Py_None == slice->start)
				from = 0;
			else 
				from = convert_index(c, slice->start);
			if (Py_None == slice->stop)
				to = c.size();
			else 
				to = convert_index(c, slice->stop);
			
            ChildrenZ* childs = new ChildrenZ();
            // Z a;
			typename std::vector<boost::shared_ptr<Z> >::iterator i;
                        
            for(i = c.begin() + from; i < c.begin() + to; i++) {
				childs->push_back(*i);
			}
                            
            return object(boost::shared_ptr<ChildrenZ>(childs));//begin()+from, begin()+to));
		}
		else {
			int index = convert_index(c, i_);
			if(index != -1) {
				return object(c[index]);
			}
			else
				return object(boost::shared_ptr<Z>());
		}
	}

	static object 
	get_slice(C &c, int from, int to)
	{
            return object();
	}
		
	/*
    void 
	set_slice(int from, int to, data_type const& v)
	{ 
		erase(begin()+from, begin()+to);
		insert(begin()+from, v);
	}
    */

	static void append(C &c, boost::shared_ptr<Z> z)
	{
        std::cout << "push_back " << z.get() << " to " << &c << std::endl;
		c.push_back(z);
	}

	static void remove(C &c, PyObject *i_) {
		if (PySlice_Check(i_)) {
			PySliceObject *slice = (PySliceObject*) i_;
			int from, to;
			if (Py_None == slice->start)
				from = 0;
			else 
				from = convert_index(c, slice->start);
			if (Py_None == slice->stop)
				to = c.size();
			else 
				to = convert_index(c, slice->stop);
			c.erase(c.begin()+from, c.begin()+to);
		}
		else {
			int index = convert_index(c, i_);
			if(index != -1) {
				c.erase(c.begin()+index);
			}
		}
	}

	~ChildrenZ() {
            C::erase(
                C::begin(),
                C::end());
	}

	static int len(C &c) {
		return c.size();
	}

	static bool contains(C &c, boost::shared_ptr<Z> z) {
		return std::find(c.begin(), c.end(), z) != c.end();
	}

	static void set_item(C &c, PyObject  *i_, boost::shared_ptr<Z> z) {
		if (PySlice_Check(i_)) {
			// PySliceObject *slice = (PySliceObject*) i_;
		}
		else {
			extract<long> _i(i_);
			if(_i.check()) {
				c[_i()] = z;
            }
		}
	}

	static void set_item2(C &c, PyObject  *i_, boost::shared_ptr<ChildrenZ> z) {
		if (PySlice_Check(i_)) {
			PySliceObject *slice = (PySliceObject*) i_;
			
			int from, to;
			if (Py_None == slice->start)
				from = 0;
			else 
				from = convert_index(c, slice->start);
			if (Py_None == slice->stop)
				to = c.size();
			else 
				to = convert_index(c, slice->stop);
			
			c.erase(c.begin()+from, c.begin()+to);
			c.insert(c.begin()+from, z->begin(), z->end());
		}
		else {
			// extract<long> _i(i_);
			// long i = _i();
			// (*this)[i] = z;
		}
	}
};

template<typename Z, typename C>
struct ChildrenZ_wrapper : public ChildrenZ<Z, C>
{
	ChildrenZ_wrapper(PyObject* self) : m_self(self) {}
	PyObject *m_self;
};

#endif
