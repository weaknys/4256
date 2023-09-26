#include "bitmap.h"
bitmap::bitmap()
{
  hdc = NULL; hbm = NULL; oldbm = NULL;
  oldpen = pen = NULL;
  oldbrush = brush = NULL;
  oldfont = font = NULL;
  //oldpen = oldbrush = oldfont = NULL;
}

bitmap::~bitmap()
{
  destroy ();
}
   
void bitmap::create(HDC CompatibleDC,int width, int height)
{
  if (hbm != NULL) destroy ();
  hdc = CreateCompatibleDC (CompatibleDC);
  hbm = CreateCompatibleBitmap (CompatibleDC, width, height); 
  oldbm = (HBITMAP) SelectObject (hdc, hbm);
  _width = width; _height = height;
  
  SetBkMode (hdc, TRANSPARENT);
}
 
void bitmap::destroy ()
{
  if (oldbrush) SelectObject (hdc, oldbrush);
  if (brush) { DeleteObject (brush); brush = NULL; }
  if (oldpen) SelectObject (hdc, oldpen);
  if (pen) { DeleteObject (pen); pen = NULL; }
  
  SelectObject (hdc, oldbm);
  DeleteObject (hbm); 
  DeleteDC (hdc);
}

void bitmap::copy_to (HDC dest, int left, int top)
{
  BitBlt (dest, left, top, _width, _height, hdc, 0, 0, SRCCOPY);
}
    
HDC bitmap::canvas()
{
  return hdc;
}

void bitmap::clear (COLORREF color)
{
  HBRUSH br = CreateSolidBrush (color);
  RECT rect; SetRect (&rect, 0, 0, _width, _height);
  FillRect (hdc, &rect, br);
  DeleteObject (br);
}

void bitmap::pen_set_solid (COLORREF color, int width)
{
  if (oldpen) SelectObject (hdc, oldpen);
  if (pen) { DeleteObject (pen); pen = NULL; }
  pen = CreatePen(PS_SOLID, width, color);
  oldpen = (HPEN) SelectObject (hdc, pen);
}

void bitmap::pen_set_null ()
{
  if (oldpen) SelectObject (hdc, oldpen);
  if (pen) { DeleteObject (pen); pen = NULL; }
  pen = (HPEN) GetStockObject (NULL_PEN);
  oldpen = (HPEN) SelectObject (hdc, pen);
}

void bitmap::brush_set_solid (COLORREF color)
{
  if (oldbrush) SelectObject (hdc, oldbrush);
  if (brush) { DeleteObject (brush); brush = NULL; }
  brush = CreateSolidBrush(color);
  oldbrush = (HBRUSH) SelectObject (hdc, brush);
}

void bitmap::brush_set_null ()
{
  if (oldbrush) SelectObject (hdc, oldbrush);
  if (brush) { DeleteObject (brush); brush = NULL; }
  brush = (HBRUSH) GetStockObject (NULL_BRUSH);
  oldbrush = (HBRUSH) SelectObject (hdc, brush);
}

void bitmap::font_set (int size, int weight, const TCHAR* fontface, COLORREF color)
{
	if (oldfont) SelectObject (hdc, oldfont);
	if (font) { DeleteObject (font); font = NULL; }
  font = CreateFont (size, 0, 0, 0, weight, false, false, false,
                      DEFAULT_CHARSET, OUT_CHARACTER_PRECIS,
											CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
											DEFAULT_PITCH | FF_DONTCARE	, fontface);
	oldfont = (HFONT) SelectObject (hdc, font);
	SetTextColor (hdc, color);
}

