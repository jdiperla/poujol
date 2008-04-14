#ifndef _GLUMOLEXCEPTION_H_
#define _GLUMOLEXCEPTION_H_

#include <string>

class GlumolException
{
public:
	GlumolException(const char *s) { message = s; }
	GlumolException(const std::string& s) { message = s; }

	std::string message;
};

#endif
