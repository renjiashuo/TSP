
#include <stdlib.h>
#include <time.h>
#include "random.h"

int roundup(float v)
{
	int u = (int)v;
	if( (v - u) > 0.0f ) u = u + 1;
	return u;
};

float random01()
{
	float r = (float)rand()/(float)RAND_MAX;
	return r;
};

float randomFloat(float low, float high)
{
	float r = low + (high - low)*random01();
	return r;
};

int randomInteger(int low, int high)
{
	int r = low + rand()%(high - low + 1);
	return r;
};