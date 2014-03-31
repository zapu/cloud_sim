#pragma once

#include <algorithm>

template<class T>
const T& clamp(const T& val, const T& lower, const T& upper)
{
	return std::min(upper, std::max(lower, val));
}

float randf();

float getRand(float lower, float upper);


