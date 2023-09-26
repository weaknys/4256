#ifndef TETRIS_H
#define TETRIS_H

#ifndef DLL_EXPORT
  #define DLL_EXPORT __declspec(dllexport)
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif

#include <windows.h>

class DLL_EXPORT fg
{
  private:
    int width, height;
    char* data;
  public:
   fg();
   fg(unsigned int iWidth, unsigned int iHeight, char value);
  ~fg();

   int Width();
   int Height();

   char* get_data () { return data; }

   char point_get(int x, int y);
   void point_set(int x, int y, char value);

   void load(const char* const src);
   void clear(char value);
   void resize(int iWidth, int iHeight, char value);
   void replace (char from, char to);

   void impose(fg &src, int x, int y, int t);
   bool cross(fg &src, int x, int y);

   void copy(fg &src);
};


#define MT_NULL				0
#define MT_BUTTON			1
#define MT_SEPARATOR		2
#define MT_OPTION			3

struct menu_item
{
	unsigned int id;
	char *name;
	int type;
	LPARAM lparam;
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


#define CELL_SIZE    0
#define FIELD_WIDTH  1
#define FIELD_HEIGHT 2

void optInit();
void optDone ();
void optDefaults ();

int optGetMaxValue (int i);
int optGetMinValue (int i);

int optGetValue (int i);
int optSetValue (int i, int v);
int optIncrease (int i);
int optDecrease (int i);

void panelInit(int items_count);
void panelDone();

void panelClear();
void panelRemove (int item);

void panelSetText  (int item, char* text);
void panelSetShape (int item, fg& shape);
void panelSetValue (int item, long value);
void panelSetSpacer (int item, int value);

int panelGetCount ();
int panelGetType (int item);

const char* panelGetText (int item);
fg*   panelGetShape (int item);
long panelGetValue (int item);

int conGetMode ();
fg* conGetField ();
void conSetMode (int new_mode);

void conCreateCells (int count, COLORREF *colors);

#endif
