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

#ifndef _POUJOL_H_
#define _POUJOL_H_

#ifdef _WIN32_WCE
# include "config/wce.h"
#else
# ifdef WIN32
#  include "config/windows.h"
# else
#  ifdef MAC
#   include "config/mac.h"
#  else
#   include "config/generic.h"
#  endif
# endif
#endif

#if defined(USE_BOOST)
#define BOOST_NO_MEMBER_TEMPLATE_FRIENDS
#define SHARED_PTR(x) boost::shared_ptr<x>
#define SHARED_PTR_FOO(x) SHARED_PTR(x)()
#define SPRITE_REFERENCE(x) SHARED_PTR(CGlumolSprite) &x
#define WEAK_PTR(x) boost::weak_ptr<x>
#define WEAK_PTR_FOO(x) WEAK_PTR(x)()
#define LOCK(x) x.lock()
#else
#define SHARED_PTR(x) x*
#define SHARED_PTR_FOO(x) NULL
#define SPRITE_REFERENCE(x) CGlumolSprite *x
#define WEAK_PTR(x) x *
#define WEAK_PTR_FOO(x) NULL
#define LOCK(x) x
#endif

#ifdef _DEBUG
# ifndef NO_TRACE_DELETIONS
#  define TRACE_DELETIONS
# endif
#endif

#ifndef NO_CONSOLE
# define SHOW_CONSOLE
#endif

#endif
