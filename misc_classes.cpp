#include "stdafx.h"
#include "misc_classes.h"
#include "poujol.h"

console_stdout::console_stdout()
{
	console.bexist = false ; 
	console.h_console= NULL; 
	console.size = CL_Sizef(80,125); 
	console.title = "Console"; 
}


console_stdout::~console_stdout()
{
	
}

CL_Rect To_CL_Rect(const CL_Rectf& f)
{
	CL_Rect rect;
	rect.top = int (f.top + 0.5) ; 
	rect.left = int (f.left + 0.5) ; 
	rect.right = int (f.right + 0.5) ; 
	rect.bottom = int (f.bottom + 0.5) ; 
	return rect;

}

CL_Point To_CL_Point(CL_Pointf& f)
{
	CL_Point point; 
	point.x = int (f.x + 0.5) ; 
	point.y = int (f.y + 0.5) ; 
	return point;
}

CL_Size To_CL_Size(CL_Sizef& f)
{
	CL_Size size; 
	size.width = int (f.width + 0.5) ; 
	size.height = int (f.height + 0.5) ; 
	return size;
}


void console_stdout::create()
{
	if (console.bexist)
		return ; 

	console.h_console = new CL_ConsoleWindow("Console"); 
	console.bexist=true;
	redirect_std();

}

void console_stdout::redirect_std()
{
	console.h_console->redirect_stdio(); 
}

void console_stdout::end()
{
	if (console.bexist)
		console.h_console->display_close_message();
}


void console_stdout::write(const char* msg)
{
	if(!console.bexist)
		return; 
	std::cout << msg ; 

} 
