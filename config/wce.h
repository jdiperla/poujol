#ifndef _WCE_H_
#define _WCE_H_

#define DEFAULT_SCREEN_WIDTH  200
#define DEFAULT_SCREEN_HEIGHT 200

#include <string>

namespace std
{

class Endl
{
public:
};

extern Endl endl;

class ostream
{
public:
	ostream& operator<<(const char *s);
	ostream& operator<<(int i);
	ostream& operator<<(Endl e);
	ostream& operator<<(std::string s);
	ostream& operator<<(void *p);
};

extern ostream cout;
extern ostream cerr;

}

#endif
