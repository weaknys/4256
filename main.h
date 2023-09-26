#ifndef MAIN_H
#define MAIN_H

//#define DD
#ifdef DD
#include "stdlib.h"
#define MSG(A) char buf[10]; MessageBox (NULL, itoa((A), buf,10),"",0);
#else
#define MSG(A) ;
#endif

#define MODE_GAME 1
#define MODE_MENU 2

int conGetMode ();
fg* conGetField ();
void conSetMode (int new_mode);

void conCreateCells (int count, COLORREF *colors);

#endif
