
#include "stdafx.h"
#include <boost/python.hpp>
#include <ClanLib/gui.h>
#include <ClanLib/core.h>
#include <ClanLib/guistylesilver.h> 

using namespace boost::python;

struct CL_StyleManager_wrap : public CL_StyleManager
{
	CL_StyleManager_wrap(PyObject* self_) :
		CL_StyleManager(NULL),
                self(self_) {}
	CL_StyleManager_wrap(PyObject* self_, CL_ResourceManager* resources)
		: CL_StyleManager(resources), self(self_) {}
	CL_StyleManager_wrap(CL_ResourceManager* resources)
		: CL_StyleManager(resources) {}

	virtual void connect_styles(
		const std::string &type,
		CL_Component *owner) { }

	PyObject* self;
};

struct CL_StyleSilverManager_wrap : public CL_StyleManager_Silver
{
	CL_StyleSilverManager_wrap(PyObject* self_) :
		CL_StyleManager_Silver(NULL),
                self(self_) {}
	CL_StyleSilverManager_wrap(PyObject* self_, CL_ResourceManager* resources)
		: CL_StyleManager_Silver(resources), self(self_) {}
	CL_StyleSilverManager_wrap(CL_ResourceManager* resources)
		: CL_StyleManager_Silver(resources) {}

	/*virtual void connect_styles(
		const std::string &type,
		CL_Component *owner) { }*/

	PyObject* self;
};

void export_stylemanager()
{
	class_<CL_StyleManager, bases< >, CL_StyleManager_wrap, boost::noncopyable>("StyleManager", init<CL_ResourceManager*>());
	class_<CL_StyleManager_Silver, bases< CL_StyleManager >, CL_StyleSilverManager_wrap, boost::noncopyable>("StyleManager_Silver", init<CL_ResourceManager*>());
}
