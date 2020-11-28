#include <math.h>
#include "MathFunctions.h"

double to_radians(double degrees)
	{
	return degrees * (180.0 / M_PI);
	}

double to_degrees(double radians)
	{
	return radians * (M_PI / 180.0);
	}

    