#include "options.h"

#define VALS_COUNT 3

struct {
	int val;  int max; int min; int def;

} static vals[] = {
		{  -1,       50,       5,      17 },
		{	 -1,      100,       5,      15 },
		{	 -1,       75,      15,      25 }
};

void optInit()
{
	optDefaults();
}

void optDone ()
{

}

void optDefaults ()
{
  for (int i = 0; i < VALS_COUNT; i++)
		vals[i].val = vals[i].def;
}

int optGetValue (int i)
{
	if (i < 0 || i >= VALS_COUNT)
	  return 0;
	else
	  return vals[i].val;
}

int optSetValue (int i, int v)
{
  if (i < 0 || i >= VALS_COUNT) return 0;
	if (v <= vals[i].max && v >= vals[i].min)
    vals[i].val = v;
  return vals[i].val;
}

int optIncrease (int i)
{
	return optSetValue(i, optGetValue (i) + 1);
}

int optDecrease (int i)
{
  return optSetValue(i, optGetValue (i) - 1);
}

int optGetMaxValue (int i)
{
	if (i < 0 || i >= VALS_COUNT)
	  return 0;
	else
	  return vals[i].max;
}

int optGetMinValue (int i)
{
	if (i < 0 || i >= VALS_COUNT)
	  return 0;
	else
	  return vals[i].min;
}
