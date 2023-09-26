#include "menu.h"

menu_item *items;
int menuCount;
int menuSelected;

void menuInit()
{
	items = NULL;
	menuCount = 0;
	menuSelected = 0;
}

int menuGetCount ()
{
	return menuCount;
}

void menuClear ()
{
	for (int i = 0; i < menuCount; i++)
		delete(items[i].name);
	delete items;
	menuCount = 0;
	menuSelected = 0;
}

void menuSet (menu_item *mi, int count)
{
	menuClear();
	items = mi;
	menuCount = count;
	
	menuSelected = -1;
	menuSelectNext (false);
}

menu_item* menuGetMenu ()
{
	return items;
}

void menuSelectNext (bool forward)
{
	int d = 1; if (forward) d = -1;
	int ms = menuSelected;
	int c = menuCount;
	bool b = true;

	do {
    ms += d; c--;
	  if (ms >= menuCount) ms = 0;
	  else if (ms < 0) ms = menuCount - 1;
	  switch (items[ms].type)
	  {
			case (MT_BUTTON):
        if (*((bool* ) items[ms].wParam) == true) b = false;
				break;
				
			case (MT_OPTION):
				b = false;
				break;
    }
  } while (b && c > 0);
	  
	if (c >= 0) menuSelected = ms;
}

int menuGetSelected ()
{
	return menuSelected;
}

unsigned int menuGetSelectedId ()
{
	return items[menuSelected].id;
}

menu_item* menuGetSelectedItem ()
{
	return &items[menuSelected];
}
