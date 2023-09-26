#ifndef MENU_H
#define MENU_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif

#include <windows.h>

#define MT_NULL				0
#define MT_BUTTON			1
#define MT_SEPARATOR	2
#define MT_OPTION			3

struct menu_item
{
	unsigned int id;
	TCHAR *name;
	int type;
	TCHAR *desc;
	//LPARAM lparam;
	WPARAM wParam;
};

const bool M_SHOW = true;
const WPARAM MENU_SHOW = (WPARAM) &M_SHOW;

void menuInit ();

void menuSet (menu_item *mi, int count);
void menuClear ();
void menuSelectNext (bool forward);

int menuGetCount ();
menu_item* menuGetMenu ();
int menuGetSelected ();
unsigned int menuGetSelectedId ();
menu_item* menuGetSelectedItem ();

#endif
