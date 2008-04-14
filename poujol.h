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
