#ifndef OPTIONS_H
#define OPTIONS_H

//#ifndef UNICODE
//#define UNICODE
//#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif

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

#endif //OPTIONS_H
