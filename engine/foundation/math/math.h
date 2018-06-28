#pragma once
#define PI 3.14159265358979323846
#define DEGTORAD (3.14159265358979323846 / 180)
#define RADTODEG (180 / 3.14159265358979323846)

namespace Math
{

static float InverseLerp(float a, float b, float value)
{
	if (a == b)
	{
		if (value < a)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return ((value - a) / (b - a));
	}
}

static float Lerp(float a, float b, float t)
{
	return a + t * (b - a);
}

template <class T>
//Returns the sign of T
static T Sign(const T& x)
{
	return (x == 0 ? 0 : (x > 0 ? 1 : -1));
}

static float Deg2Rad(const float& deg)
{
	return (float)(deg * (DEGTORAD));
}

static float Rad2Deg(const float& rad)
{
	return (float)(rad * (RADTODEG));
}

}



