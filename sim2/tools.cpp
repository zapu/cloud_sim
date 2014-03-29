#include "tools.h"

#include <cstdlib>

float randf()
{
	return (float)rand() / RAND_MAX;
}

float getRand( float lower, float upper )
{
	float d = upper - lower;
	return lower + randf() * d;
}
