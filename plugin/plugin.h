#ifndef PLUGIN_H
#define PLUGIN_H

//#ifndef UNICODE
//#define UNICODE
//#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif

#include "fg.h"
#include "panel.h"
#include "main.h"

#define action_left     1
#define action_up       2
#define action_right    3
#define action_down     4
#define action_special1 5
#define action_special2 6
#define action_timer    7

#define event_none      0
#define event_newgame   1

#define IF_TITLE        0
#define IF_ID           1
#define IF_MAXPLAYERS  2

void plInit (int field_width, int field_height, int players_count);
void plStop ();

int  plAction (int action);
void plUpdateField ();
void plRestart ();

LPARAM plInfo (int field);

#endif //PLUGIN_H
