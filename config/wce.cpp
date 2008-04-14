#include "wce.h"

namespace std
{

ostream& ostream::operator<<(const char *s)
{
	printf("%s", s);
	return *this;
}

ostream& ostream::operator<<(int i)
{
	printf("%d", i);
	return *this;
}

ostream& ostream::operator<<(Endl e)
{
	printf("\n");
	return *this;
}

ostream& ostream::operator<<(std::string s)
{
	printf("%s", s.c_str());
	return *this;
}

ostream& ostream::operator<<(void *p)
{
	printf("%p", p);
	return *this;
}

ostream cout;
ostream cerr;
Endl endl;

}