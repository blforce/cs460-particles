
#ifndef CUSTOMMATH_H
#define	CUSTOMMATH_H

#include "Types.h"
#include "constants.h"
#include <math.h>
#include <cstdlib>
#include <time.h>

class Math
{
public:

	static float Distance(Vector start, Vector end)
	{
		return sqrt(pow(end.x - start.x, 2) + 
				pow(end.y - start.y, 2) +
				pow(end.z - start.z, 2));
	}

	static float RandomValue(float min, float max)
	{
		float random = (float)rand() / (float) RAND_MAX;

		return (random * (max - min)) + min;
	}

	static float dT(int startTime, int endTime)
	{
		return (float)(endTime - startTime) / (float)CLOCKS_PER_SEC;
	}

	static float ElapsedSeconds(int startTime)
	{
		return dT(startTime, clock());
	}
};

#endif