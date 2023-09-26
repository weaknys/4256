#include "skin.h"
#include <stdlib.h>

const TCHAR CLASSNAME[] = TEXT("Tetris");
const TCHAR font_face[] = TEXT("Calibri");

#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED     0x00080000
#define LWA_COLORKEY 0x00000001
#endif // ndef WS_EX_LAYERED
#define LWA_ALPHA     0x00000002

#define CLOSE_VAR 0

typedef BOOL (WINAPI *lpfnSetLayeredWindowAttributes)(HWND hWnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags);
lpfnSetLayeredWindowAttributes SetLayeredWindowAttributes;
BOOL nullSetLayeredWindowAttributes (HWND hWnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags)
{
	return true;
}

struct {
  bitmap back, *cell, buffer;
  
  int width, height;
  int caption_size;
  int border_size;
  int cell_padding;
  int panel_width;
  int colors_count;
  
  WNDCLASS wincl;
	HWND hwnd;
	HCURSOR cursor;
	HRGN rgn;
	
	RECT close_rect;
} skin_vars;

void skinInitVars ();
void skinCreateCells (int count, COLORREF *_colors);
void skinMakeBar (bitmap& bmp, PRECT rect, COLORREF color, int v = 30);
void skinUpdateWindow (HWND hwnd);

LRESULT CALLBACK wndproc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  PAINTSTRUCT ps; HDC hdc; POINT pt;
  switch (message)
  {
    case WM_PAINT:
      hdc = BeginPaint(hwnd, &ps);
      skin_vars.buffer.copy_to (hdc, 0, 0);
      EndPaint(hwnd, &ps);
      break;
    case WM_LBUTTONDOWN:
			pt.x = LOWORD(lParam); pt.y = HIWORD(lParam);
			if (PtInRect (&(skin_vars.close_rect), pt))
			  PostQuitMessage (0);
			else  
        PostMessage(hwnd, WM_SYSCOMMAND, 0xf012, 0); 
      break;  
    case WM_RBUTTONUP:
    case WM_DESTROY:  
        PostQuitMessage (0);
      break;
      
    case WM_KILLFOCUS:
			conSetMode (MODE_MENU);
			SetLayeredWindowAttributes(skin_vars.hwnd, 0, 200, LWA_ALPHA);
			UpdateWindow(hwnd);
			break;
			
		case WM_SETFOCUS:
			SetLayeredWindowAttributes(skin_vars.hwnd, 0, 255, LWA_ALPHA);
			UpdateWindow(hwnd);
			break;

		case WM_MOUSEMOVE:
      if (skin_vars.cursor != NULL)
			{
				SetCursor (skin_vars.cursor);
				skin_vars.cursor = NULL;
		  }
		break;
		case WM_KEYDOWN:
			if (skin_vars.cursor == NULL) skin_vars.cursor = SetCursor (NULL);
		break;
		
    case WM_CREATE:
      skinUpdateWindow(hwnd);
        break;
    default:
      return DefWindowProc (hwnd, message, wParam, lParam);
  }
  return 0;
}

int skinInit (HINSTANCE hInst)
{
	skin_vars.cell = NULL;
	skin_vars.colors_count = 0;

	skinInitVars();

  skin_vars.wincl.hInstance = hInst; 
  skin_vars.wincl.lpszClassName = CLASSNAME;
  skin_vars.wincl.lpfnWndProc = wndproc; 
  skin_vars.wincl.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  skin_vars.wincl.hIcon = LoadIcon (hInst, TEXT("A"));
  skin_vars.wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
  skin_vars.wincl.lpszMenuName = NULL; 
  skin_vars.wincl.cbClsExtra = 0;
  skin_vars.wincl.cbWndExtra = 0; 
  skin_vars.wincl.hbrBackground = NULL;
  
  if (RegisterClass(&(skin_vars.wincl)) == 0)
	  return 0;

  HMODULE hUser32 = GetModuleHandle(TEXT("USER32.DLL"));
  SetLayeredWindowAttributes =
	  (lpfnSetLayeredWindowAttributes)GetProcAddress(hUser32, "SetLayeredWindowAttributes");
	if (NULL == SetLayeredWindowAttributes)
    SetLayeredWindowAttributes = (lpfnSetLayeredWindowAttributes)nullSetLayeredWindowAttributes;

	int left = (GetSystemMetrics(SM_CXSCREEN) - skin_vars.width)  / 2;
	int top  = (GetSystemMetrics(SM_CYSCREEN) - skin_vars.height) / 2;
	skin_vars.hwnd = CreateWindowEx (WS_EX_LAYERED, CLASSNAME,
	                          TEXT("Tetris"), WS_POPUP,
                            left, top, skin_vars.width, skin_vars.height,
                            NULL, NULL, hInst, NULL);
  if (skin_vars.hwnd == NULL) return 0;
  //SetLayeredWindowAttributes(skin_vars.hwnd, 0, 255, LWA_ALPHA);
  ShowWindow (skin_vars.hwnd, SW_SHOW);    
  UpdateWindow(skin_vars.hwnd);  
  
  return 1;
}


void skinInitVars ()
{
  skin_vars.caption_size = 20;
  skin_vars.border_size = 3;
  skin_vars.cell_padding = (int) (optGetValue(CELL_SIZE) * 0.1);
  skin_vars.panel_width = (skin_vars.cell_padding + optGetValue(CELL_SIZE)) * 8;

  skin_vars.width  = optGetValue(FIELD_WIDTH) *
                     (optGetValue(CELL_SIZE) + skin_vars.cell_padding) +
                     skin_vars.border_size*2 +
                     skin_vars.cell_padding +
										 skin_vars.panel_width;
  skin_vars.height = optGetValue(FIELD_HEIGHT) *
                     (optGetValue(CELL_SIZE) + skin_vars.cell_padding) +
                     skin_vars.caption_size +
                     skin_vars.border_size*2 + 
                     skin_vars.cell_padding;

	if (CLOSE_VAR)
	{
	SetRect (&(skin_vars.close_rect),
	         skin_vars.width - 26, -2, skin_vars.width - 7, 17);
	}
	else
	{
  SetRect (&(skin_vars.close_rect),
	         skin_vars.width - 23, 3, skin_vars.width - 4, 22);
	}

	skin_vars.cursor = NULL;
}

void skinCreateCells (int count, COLORREF *colors)
{
  int cell_size = optGetValue(CELL_SIZE);
  HDC hdc = GetDC (skin_vars.hwnd);
  
  if (skin_vars.cell)
	{
		for (int i = 0; i < skin_vars.colors_count; i++)
			skin_vars.cell[i].destroy();
		delete skin_vars.cell;
  }
	skin_vars.cell = new bitmap[count];
  for (int i = 0; i < count; i++)
  {
    skin_vars.cell[i].create(hdc,cell_size,cell_size);
    skinMakeBar (skin_vars.cell[i], NULL, colors[i]);
  }   
	skin_vars.colors_count = count;
}

void skinMakeBar (bitmap& bmp, PRECT rect, COLORREF color, int v)
{
  int w, h;
  PRECT prect;
  if (rect == NULL)
  {
    prect = new RECT;
    w = bmp.width();
    h = bmp.height();
    SetRect (prect, 0, 0, w, h);
  }  
  else
  {
    prect = rect;
    w = rect->right - rect->left; if (w < 0) w = -w;
    h = rect->bottom - rect->top; if (h < 0) h = -h;
  }  
  
  BYTE red   = GetRValue(color);
  BYTE green = GetGValue(color);
  BYTE blue  = GetBValue(color);
   
	int d; 
	for (int i = prect->top; i <= prect->bottom; i++) 
  {
    double y = ((double) i-prect->top)/((double) h);
    d = (int) (v - y*y * v);
    
    int r = red   + d; if (r > 255) r = 255; else if (r < 0) r = 0;
    int g = green + d; if (g > 255) g = 255; else if (g < 0) g = 0;
    int b = blue  + d; if (b > 255) b = 255; else if (b < 0) b = 0;
    
    bmp.pen_set_solid (RGB(r,g,b));
    MoveToEx (bmp.canvas(), prect->left,  i, NULL);
    LineTo   (bmp.canvas(), prect->right, i);
  } 
}

void skinUpdateScreen ()
{
  skin_vars.back.copy_to (skin_vars.buffer.canvas(), 0, 0);

	int mode = conGetMode();
	RECT rect;
	TCHAR buf[128];
	
	if (mode == MODE_GAME)
	{
  	int fwidth  = optGetValue(FIELD_WIDTH);
		int fheight = optGetValue(FIELD_HEIGHT);
  	int cell_size = optGetValue(CELL_SIZE)+skin_vars.cell_padding;
  	int x_offset = skin_vars.border_size + skin_vars.cell_padding;
  	int y_offset = skin_vars.border_size + skin_vars.caption_size +
                 	 skin_vars.cell_padding;

		int cell_val;
		fg *shape = conGetField();

  	for (int i = 0; i < fwidth;  i++)
  	for (int j = 0; j < fheight; j++)
  	{
			cell_val = shape->point_get (i+1, j+1);
			if (cell_val != 0)
      	skin_vars.cell[cell_val-1].copy_to (skin_vars.buffer.canvas(),
			  	                                  i*cell_size + x_offset,
																						j*cell_size + y_offset);
		}

		shape = NULL;

  	skin_vars.buffer.font_set (18, FW_NORMAL, font_face, RGB(255,255,255));

		SetRect (&rect, skin_vars.width - skin_vars.panel_width + 7,
    	              skin_vars.caption_size + 15,
						      	skin_vars.width - 10,
      							skin_vars.height - 10);

		for (int i = 0; i < panelGetCount(); i++)
	  	switch (panelGetType(i))
	  	{
		  	case PT_TEXT:
					rect.top += DrawText (skin_vars.buffer.canvas(),
				  	                    panelGetText(i), -1, &rect, DT_WORDBREAK);
					rect.top += 10;
				break;
				case PT_SHAPE:
					shape = panelGetShape (i);

					for (int i1 = 0; i1 < shape->Width();  i1++)
        	for (int j1 = 0; j1 < shape->Height(); j1++)
        	{
      			cell_val = shape->point_get (i1+1, j1+1);
       			if (cell_val != 0)
           	  skin_vars.cell[cell_val-1].copy_to (
						  	    								    skin_vars.buffer.canvas(),
			          	      								i1*cell_size + rect.left,
																				j1*cell_size + rect.top);
	      	}
	      	rect.top += shape->Height() * cell_size +10;
				break;
				case PT_VALUE:
					DrawText (skin_vars.buffer.canvas(),
				 		        panelGetText(i), -1, &rect, DT_LEFT);
					rect.top += DrawText (skin_vars.buffer.canvas(),
				  	        itoa(panelGetValue(i),buf,10), -1, &rect, DT_RIGHT);
					rect.top += 10;
				break;
				case PT_SPACER:
					rect.top += panelGetValue(i);
				break;
			}
    	shape = NULL;
  }
  
	else if (mode == MODE_MENU)
	{
		RECT panelRect;
		SetRect (&panelRect, skin_vars.width - skin_vars.panel_width + 7,
												 skin_vars.caption_size + 15,
												 skin_vars.width - 10,
												 skin_vars.height - 10);
												 
  	skin_vars.buffer.font_set (26, FW_NORMAL, font_face, RGB(70,145,230));

		SetRect (&rect,
	  	       skin_vars.border_size + 30,
      	     skin_vars.caption_size + skin_vars.border_size + 50,
        	   skin_vars.width - skin_vars.border_size - skin_vars.panel_width - 50,
          	 skin_vars.height-skin_vars.border_size - 30);
	
		int n = menuGetCount();
		int s = menuGetSelected();
		menu_item* mi = menuGetMenu();
		for (int i = 0; i < n; i++)
		{
    	if (s == i)
			{
				if (mi[i].type == MT_BUTTON || mi[i].type == MT_OPTION)
				{
          skin_vars.buffer.font_set (20, FW_NORMAL, font_face, RGB(210,220,255));
					int h = DrawText (skin_vars.buffer.canvas(), mi[i].desc, -1,
					    				&panelRect, DT_WORDBREAK | DT_CALCRECT);
					panelRect.bottom = skin_vars.height - skin_vars.border_size - 10;
					panelRect.top = panelRect.bottom - h;
					DrawText (skin_vars.buffer.canvas(), mi[i].desc, -1,
					    				&panelRect, DT_WORDBREAK);
			  }
				skin_vars.buffer.font_set (26, FW_NORMAL,font_face, RGB(230,145,70));
		  }
	  	switch (mi[i].type)
	  	{
		  	case MT_BUTTON:
					if (*((bool *) mi[i].wParam) == true)
					{
            rect.top += DrawText (skin_vars.buffer.canvas(),
																	mi[i].name, -1, &rect, DT_LEFT);
			  		rect.top += 3;
					}
		  	break;
		  	
		  	case MT_SEPARATOR:
			  	rect.top += 10;
		  	break;
		  	
		  	case MT_OPTION:
          DrawText (skin_vars.buffer.canvas(),
										mi[i].name, -1, &rect, DT_LEFT);
					rect.top += DrawText (skin_vars.buffer.canvas(),
																itoa(optGetValue((int) mi[i].wParam), buf, 10),
																-1, &rect, DT_RIGHT);
			  	rect.top += 3;
				break;
    	}
    	if (s == i) skin_vars.buffer.font_set (26, FW_NORMAL,
		                                       font_face, RGB(70,145,230));
  	}
	}
  InvalidateRect (skin_vars.hwnd, NULL, false);
}

void skinRebuild()
{
  skinInitVars();
	
  MoveWindow (skin_vars.hwnd,
	            (GetSystemMetrics(SM_CXSCREEN) - skin_vars.width)  / 2,
	            (GetSystemMetrics(SM_CYSCREEN) - skin_vars.height) / 2,
	            skin_vars.width, skin_vars.height,
							true);
	skinUpdateWindow (skin_vars.hwnd);
}

void skinUpdateWindow (HWND hwnd)
{
	if (skin_vars.rgn) DeleteObject (skin_vars.rgn);
	skin_vars.rgn = CreateRoundRectRgn (0,0,
	                           skin_vars.width+1, skin_vars.height+1,5,5);
  SetWindowRgn (hwnd, skin_vars.rgn, true);
	UpdateWindow(skin_vars.hwnd);

  HDC hdc = GetDC (hwnd);
  skin_vars.buffer.create (hdc, skin_vars.width, skin_vars.height);
  skin_vars.back.create (hdc, skin_vars.width, skin_vars.height);

  RECT r; SetRect (&r, 0, 0, skin_vars.width, skin_vars.height);
  skinMakeBar (skin_vars.back, &r, RGB(70, 145, 230));

  skin_vars.back.pen_set_solid (RGB(70, 145, 230));
  skin_vars.back.brush_set_null();
  RoundRect (skin_vars.back.canvas(),
             0, 0, skin_vars.width, skin_vars.height, 5, 5);

	if (CLOSE_VAR)
	{
	RoundRect (skin_vars.back.canvas(),
             skin_vars.close_rect.left, skin_vars.close_rect.top,
             skin_vars.close_rect.right, skin_vars.close_rect.bottom,
						 5, 5);
  
  skin_vars.back.pen_set_solid (RGB (210, 220, 255), 3);
  }
  else
  {
	skin_vars.back.pen_set_solid (RGB (255, 255, 255), 3);
	}
	
  MoveToEx (skin_vars.back.canvas(),
	skin_vars.close_rect.left + 6,  skin_vars.close_rect.top + 6, NULL);
	LineTo   (skin_vars.back.canvas(),
	skin_vars.close_rect.right - 7,  skin_vars.close_rect.bottom - 6);
	MoveToEx (skin_vars.back.canvas(),
	skin_vars.close_rect.right - 7,  skin_vars.close_rect.top + 6, NULL);
	LineTo   (skin_vars.back.canvas(),
	skin_vars.close_rect.left + 6, skin_vars.close_rect.bottom - 6);

  skin_vars.back.pen_set_solid (RGB(70, 145, 230));
  skin_vars.back.brush_set_solid (RGB(255,255,255));
  RoundRect (skin_vars.back.canvas(),
             skin_vars.border_size - 1,
             skin_vars.caption_size + skin_vars.border_size - 1,
             skin_vars.width - skin_vars.border_size - skin_vars.panel_width + 1,
             skin_vars.height-skin_vars.border_size + 1,
             5, 5);

  skin_vars.back.font_set (18, FW_BOLD);
  
  if (CLOSE_VAR)
  SetTextColor (skin_vars.back.canvas(), RGB(250,250,255));
  else
  SetTextColor (skin_vars.back.canvas(), RGB(255,255,255));

  SetRect (&r, skin_vars.border_size + 10,
	             skin_vars.border_size,
							 skin_vars.close_rect.left - 10,
      				 skin_vars.caption_size);
  DrawText (skin_vars.back.canvas(), "Тетрис", -1, &r, DT_LEFT);

  SetTextColor (skin_vars.buffer.canvas(), RGB(250,250,255));
  skin_vars.buffer.font_set ();
  skin_vars.back.copy_to (skin_vars.buffer.canvas(), 0, 0);
        
}
