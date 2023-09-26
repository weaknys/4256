#ifndef PANEL_H
#define PANEL_H

//#ifndef UNICODE
//#define UNICODE
//#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif

#include <windows.h>
#include "fg.h"

#define PT_NULL   0
#define PT_TEXT   1
#define PT_SHAPE  2
#define PT_VALUE  3
#define PT_SPACER 4

void panelInit(int items_count);
void panelDone();
   
void panelClear();
void panelRemove (int item);
    
void panelSetText  (int item, TCHAR* text);
void panelSetShape (int item, fg& shape);
void panelSetValue (int item, long value);
void panelSetSpacer (int item, int value);
    
int panelGetCount ();
int panelGetType (int item);
    
const TCHAR* panelGetText (int item);
fg*   panelGetShape (int item);
long panelGetValue (int item);

#endif
