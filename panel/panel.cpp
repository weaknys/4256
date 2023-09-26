#include "panel.h"

struct pItem
{
 	int type;
  TCHAR* string;
  fg *shape;
  long val;
} *pitems;

int panelCount;

void panelInit (int items_count)
{
	if (items_count <= 0 || items_count > 10) return;
	panelCount = items_count;
	pitems = new pItem[items_count];
	for (int i = 0; i < items_count; i++)
	  pitems[i].type = PT_NULL;
}

void panelDone ()
{
  panelClear();
}

void panelClear()
{
	for (int i = 0; i < panelCount; i++)
	  panelRemove(i);
	delete pitems;	
}

void panelRemove (int item)
{
  if (item < 0 || item >= panelCount) return;
	switch (pitems[item].type)
  {
	  case PT_TEXT:  
			delete pitems[item].string; 
			pitems[item].string = NULL; 
			break;
	  case PT_SHAPE: 
			delete pitems[item].shape; 
			pitems[item].shape = NULL;
			break;
		case PT_VALUE:
      delete pitems[item].string;
      pitems[item].string = NULL;
      pitems[item].val = 0;
      break;
    case PT_SPACER:
      pitems[item].val = 0;
      break;
	}
	pitems[item].type = PT_NULL;
}

void panelSetText  (int item, TCHAR* text)
{
  if (item < 0 || item >= panelCount) return;
	if (pitems[item].type == PT_VALUE) 
	  delete pitems[item].string;
	else
	{
	  panelRemove (item);
		pitems[item].type = PT_TEXT;
  }	
	pitems[item].string = new TCHAR[lstrlen(text)+1];
	lstrcpy (pitems[item].string, text);
}

void panelSetShape (int item, fg& shape)
{
  if (item < 0 || item >= panelCount) return;
	panelRemove (item);
	pitems[item].type = PT_SHAPE;
	pitems[item].shape = new fg;
	pitems[item].shape->copy (shape);
}

void panelSetValue (int item, long value)
{
	if (item < 0 || item >= panelCount) return;
	if (pitems[item].type != PT_VALUE)
	  panelRemove (item);
	pitems[item].type = PT_VALUE;
	pitems[item].val = value;
}

void panelSetSpacer (int item, int value)
{
  if (item < 0 || item >= panelCount) return;
	panelRemove (item);
	pitems[item].type = PT_SPACER;
	pitems[item].val = value;
}

int panelGetCount()
{
  return panelCount;
}

int panelGetType (int item)
{
  if (item < 0 || item >= panelCount) return -1;
  return pitems[item].type;
}

const TCHAR* panelGetText (int item)
{
  if (item < 0 || item >= panelCount) return NULL;
	if (panelGetType(item) == PT_TEXT || panelGetType(item) == PT_VALUE)
	  return pitems[item].string;
	else
	  return NULL;
}

fg* panelGetShape (int item)
{
  if (item < 0 || item >= panelCount) return NULL;
	if (panelGetType(item) == PT_SHAPE)
	  return pitems[item].shape;
	else
	  return NULL;
}

long panelGetValue (int item)
{
  if (item < 0 || item >= panelCount) return 0;
	if (panelGetType(item) == PT_VALUE || panelGetType(item) == PT_SPACER)
	  return pitems[item].val;
	else
	  return 0;
}
