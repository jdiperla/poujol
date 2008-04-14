#ifndef _MISC_CLASSES_H_
#define _MISC_CLASSES_H_

#include <ClanLib/core.h>
#include <ClanLib/display.h>

struct sconsole
{
	bool bexist;
	const char* title;
	CL_Sizef size;
	CL_ConsoleWindow* h_console;

};

class console_stdout
{
public :
	sconsole console;

	console_stdout();
	~console_stdout();

	void redirect_std();
	void create();
	void end();
	void write(const char* msg);
 };

CL_Rect To_CL_Rect(const CL_Rectf& f);
CL_Point To_CL_Point(CL_Pointf& f);
CL_Size To_CL_Size(CL_Sizef& f);

enum rotation_dir
{
	clockwise,
	counterclockwise
};

enum play_mode {
	loop,
	pingpong
};

#endif
