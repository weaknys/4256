#include "plugin.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

fg back, shape, tmp_shape, next_shape;
int left, top;
int lines;

static char f_map[] = {0,6,12,20,28,36,44};
static char f[] = {
             2,2,1,1,1,1,
             1,4,1,1,1,1,
             2,3,1,1,0,1,0,1,
             2,3,1,1,1,0,1,0,
             3,2,1,1,0,0,1,1,
             3,2,0,1,1,1,1,0,
             3,2,0,1,0,1,1,1
          };
#define N_FIGS (sizeof(f_map)/sizeof(f_map[0]))          

void rotate ();
void make_level(int p);
void new_shape ();
bool next();

void plInit (int field_width, int field_height, int players_count)
{
  srand((unsigned)time( NULL ));
  back.resize (field_width, field_height, 0);
  shape.resize(0,0,0);
  next_shape.load(f+f_map[rand()%N_FIGS]);
  next_shape.replace(1, 3);
  lines = 0;
  panelSetText  (0, "Далее");
  panelSetValue (2, lines);
  panelSetText  (2, "Линии");
  
  static COLORREF colors[] = {
         RGB ( 70, 145, 230), //blue
         RGB ( 91, 182,  81), //green
         //RGB (230, 145,  70),  //orange
         RGB(210,220,255)   //white-white blue
         //RGB (215, 233, 255)
         //RGB (255, 255, 200)
         //RGB (255, 194, 166)
         //RGB (80, 80, 80)
      };
  conCreateCells (3, colors);
}

void plStop ()
{
  
}

int plAction (int action)
{
	int ret = event_none;
  switch (action)
  {
    case action_left:
      if (back.cross(shape, left-1, top) == true) left--;
     break;
    case action_right:
      if (back.cross(shape, left+1, top) == true) left++;
     break;
    case action_up:
        rotate();
        if (back.cross(tmp_shape, left, top) == true)
           shape.copy(tmp_shape);
      break;
    case action_timer:  
    case action_down:
      if (back.cross(shape, left, top+1) == true) 
        top++;
      else
      {
        if (next())
          ret = event_newgame;
		  }  
        
      break;
    case action_special1:
			while (back.cross(shape, left, top+1) == true) 
			  top++;
      if (next())
        ret = event_newgame;
			break;  
   }
   plUpdateField ();
   return ret;
}

void plUpdateField ()
{
	fg* field = conGetField ();
  field->copy(back);
  field->impose(shape, left, top, 0);
}

void plRestart ()
{
  new_shape ();
  back.clear(0);
  make_level(65);
  plUpdateField();
}

void rotate ()
{
  tmp_shape.resize(shape.Height(), shape.Width(), 0);

  for (int i = 1; i <= shape.Width(); i++)
  for (int j = 1; j <= shape.Height(); j++)
    tmp_shape.point_set(j,i,shape.point_get(shape.Width()-i+1,j));
}

bool next()
{
  shape.replace (2,1);
  back.impose(shape,left,top,0);

  for (int j = back.Height(); j > 0;)
  {
    bool f = true;
    for (int i = 1; i <= back.Width() && true == f; i++)
      if (back.point_get(i,j) == 0) f = false;

    if (true == f)
    {
      for (int j2 = j - 1; j2 > 0; j2--)
      for (int i2 = 1; i2 <= back.Width(); i2++)
        back.point_set(i2, j2 + 1, back.point_get(i2, j2));
      lines++;
      panelSetValue (2, lines);
    }    
    else
      j--;
  }

  new_shape();
  if (back.cross(shape, left, top) != true)
  {
    plRestart();
    return true;
 }
 return false;
}

void make_level(int p)
{
  for (int j = (int) (back.Height()*0.8); j <= back.Height(); j++)
  {
    bool t1 = false;
    for (int i = 1; i <= back.Width(); i++)
    {
      int t2;
      if (rand()%100 < p) t2 = 1; else t2 = 0;
      back.point_set(i, j, t2);
      if (t2 == 0) t1 = true;
    }  
    if (t1 == false) back.point_set((rand()%back.Width())+1, j, 0);
  }  
}

void new_shape ()
{
  left = back.Width()/2; top = 1;  // shape position
  shape.copy(next_shape);
  shape.replace (3,2);
  next_shape.load(f+f_map[rand()%N_FIGS]);
  next_shape.replace(1, 3);
  
  tmp_shape.resize(8, 4, 0);
	tmp_shape.impose(next_shape,
	                 (8-next_shape.Width())/2,
									 0,
									 false);
	panelSetShape (1, tmp_shape);
  
}    

LPARAM plInfo (int field)
{
	switch (field)
	{
		case IF_TITLE:
			return (LPARAM) TEXT("Tetris");
			break;
		case IF_ID:
			return (LPARAM) TEXT("TETRIS 0.1");
			break;
		case IF_MAXPLAYERS:
			return (LPARAM) conGetField()->Width() / 6;
			break;
  }
  return (LPARAM) 0;
}
