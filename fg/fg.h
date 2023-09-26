#ifndef FG_H
#define FG_H

//#ifndef UNICODE
//#define UNICODE
//#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif

#include <windows.h>

class fg
{
  private:
    int width, height;
    char* data;
  public:
   fg();
   fg(unsigned int iWidth, unsigned int iHeight, char value);
  ~fg();

   int Width();
   int Height();
 
   char* get_data () { return data; }

   char point_get(int x, int y);
   void point_set(int x, int y, char value);

   void load(const char* const src);
   void clear(char value);
   void resize(int iWidth, int iHeight, char value);
   void replace (char from, char to);

   void impose(fg &src, int x, int y, int t);
   bool cross(fg &src, int x, int y);

   void copy(fg &src);
};

#endif //FG_H


