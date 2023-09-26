#ifndef BITMAP_H
#define BITMAP_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif

#include <windows.h>

class bitmap
{
    HBITMAP hbm, oldbm;
    HDC hdc;

    HPEN pen, oldpen;
    HBRUSH brush, oldbrush;
    HFONT font, oldfont;
    //HGDIOBJ oldpen, oldbrush, oldfont;
    
    int _width, _height;
  public:
    bitmap();
   ~bitmap();

    void create(HDC ,int width, int height);
    void destroy ();
    void copy_to (HDC , int left, int top);
    void clear (COLORREF );
    
    void pen_set_solid (COLORREF color, int width = 1);
    void pen_set_null ();
    void brush_set_solid (COLORREF color);
    void brush_set_null ();
    
    void font_set (int size = 18, int weight = FW_NORMAL,
		               const TCHAR* fontface = "Calibri",
									 COLORREF color = RGB(255, 255, 255));
		               

    HDC canvas();
    
    int width() {return _width; }
    int height() {return _height; }
};

#endif //BITMAP_H
