#ifndef SKIN_H
#define SKIN_H

//#ifndef UNICODE
//#define UNICODE
//#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif

#include <windows.h>
#include "bitmap.h"
#include "panel.h"
#include "options.h"
#include "fg.h"
#include "menu.h"
#include "main.h"

int skinInit (HINSTANCE);
void skinUpdateScreen ();
void skinCreateCells (int count, COLORREF *_colors);
void skinRebuild();

#endif
