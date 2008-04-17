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