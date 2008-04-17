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
