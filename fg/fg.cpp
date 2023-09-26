#include "fg.h"

fg::fg()
{
  width = 0; height = 0; data = NULL;
}

fg::fg(unsigned int iWidth, unsigned int iHeight, char value)
{
  width = iWidth; height = iHeight;
  data = new char[iWidth*iHeight];
  FillMemory (data, iWidth*iHeight, value);
}

fg::~fg()
{
  delete data;
}

void fg::clear(char value)
{
  FillMemory (data, width*height, value);
}

char fg::point_get(int x, int y)
{
  if (x > width || x < 1 || y > height || y < 1)  return -1;
  return data[(y-1)*width+(x-1)];
}

void fg::point_set(int x, int y, char value)
{
  if (x > width || x < 1 || y > height || y < 1)  return ;
  data[(y-1)*width+(x-1)] = value;
}

void fg::load(const char* const src)
{
  if (src[0] != width || src[1] != height)
  {
    width = src[0];
    height = src[1];
    if (data) delete data;
	data = new char[src[0]*src[1]];
  }
  CopyMemory (data, src+2, src[0]*src[1]);
}

void fg::impose(fg &src, int x, int y, int t)
{
  if (t == 0)
    for (int i = 1; i <= src.width; i++)
    for (int j = 1; j <= src.height; j++)
    {
      char a = src.point_get(i,j);
      if (a > 0)
        this->point_set(i+x, j+y, a);
    }
  else
    for (int i = 1; i <= src.width; i++)
    for (int j = 1; j <= src.height; j++)
      this->point_set(i+x, j+y, src.point_get(i,j));
}

bool fg::cross(fg &src, int x, int y)
{
  bool f = true;
  for (int i = 1; i <= src.width; i++)
  for (int j = 1; j <= src.height; j++)
    if (this->point_get(i+x, j+y) != 0 && src.point_get(i, j) != 0) f = false;
  return f;
}

void fg::resize(int iWidth, int iHeight, char value)
{
  if (width != iWidth || height != iHeight)
  {
    if (data) delete data;
    data = new char[iWidth*iHeight];
    width = iWidth; height = iHeight;
  }  
  FillMemory (data, iWidth*iHeight, value);
}

void fg::copy(fg &src)
{
  if (width != src.width || height != src.height)
  {
    if (data) delete data;
    width = src.width; height = src.height;
    data = new char[src.width*src.height];
  }	
  CopyMemory (data, src.data, src.width*src.height);
}

int fg::Width()
{
  return width;
}

int fg::Height()
{
  return height;
}

void fg::replace (char from, char to)
{
  for (int i = 0; i < width*height; i++)
    if (data[i] == from) data[i] = to;
}
