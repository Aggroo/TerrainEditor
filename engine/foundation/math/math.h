#pragma once
#define M_EPSILON = 0.000001f;
#define PI 3.14159265358979323846
#define DEGTORAD (3.14159265358979323846 / 180)
#define RADTODEG (180 / 3.14159265358979323846)
#include "vec4.h"

namespace Math
{

static float InverseLerp(const float a, const float b, const float value)
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
		return (value - a) / (b - a);
	}
}

static float Lerp(const float a, const float b, const float t)
{
	return a + t * (b - a);
}

template <class T>
//Returns the sign of T
static T Sign(const T& x)
{
	return (x == 0 ? 0 : (x > 0 ? 1 : -1));
}

template <class T> constexpr const T& Max(const T& a, const T& b) {
	return (a < b) ? b : a;
}

template <class T> constexpr const T& Min(const T& a, const T& b) {
	return !(b < a) ? a : b;
}

template <typename T>
static T Clamp(T x, T a, T b) { return x < a ? a : (x > b ? b : x); }


static float Deg2Rad(const float& deg)
{
	return (float)(deg * (DEGTORAD));
}

static float Rad2Deg(const float& rad)
{
	return (float)(rad * (RADTODEG));
}

static int Abs(const int& x)
{
	int m = x >> 31;
	return((x ^ m) - m);
}

static int Nabs(const int& x)
{
	int m = x >> 31;
	return(m - (x ^ m));
}

static int Sign(const int& x)
{
	return ((x >> 31) - (-x >> 31));
}

static bool IsPowerOfTwo(int x)
{
	int y = x - 1; // y is negative only if x == 0.
	return ((x & y) - (y >> 31) == 0);
}

static void Barycentric(const vec4& p, const vec4& a, const vec4& b, const vec4& c, float& u, float& v, float& w)
{
	vec4 v0 = b - a;
	vec4 v1 = c - a;
	vec4 v2 = p - a;

	float d00 = Math::vec4::Dot(v0, v0);
	float d01 = Math::vec4::Dot(v0, v1);
	float d11 = Math::vec4::Dot(v1, v1);
	float d20 = Math::vec4::Dot(v2, v0);
	float d21 = Math::vec4::Dot(v2, v1);

	float denom = 1 / (d00 * d11 - d01 * d01);

	v = (d11 * d20 - d01 * d21) * denom;
	w = (d00 * d21 - d01 * d20) * denom;

	u = 1.0f - v - w;
}

//returns a random float between zero and 1 
inline float RandFloat()
{
	return ((rand()) / (RAND_MAX + 1.0f));
}
//returns a random float between a and b 
inline float RandFloat(float a, float b)
{
	if (a > b)
	{
		float temp = a;
		a = b;
		b = temp;
	}
	float r = (float)rand() / (float)RAND_MAX;
	return a + r * (b - a);

}

//returns a random float in the range -1 < n < 1 
inline float randomClamped() { return RandFloat() - RandFloat(); }

inline void RandomPointInCircle(const float& radius, float& x, float& y)
{
	float angle = RandFloat() * (float)PI * 2;
	float randRad = sqrtf(RandFloat()) * radius;
	x = randRad * cosf(angle);
	y = randRad * sinf(angle);
}

inline void RandomPointInSphere(const float& radius, Math::vec4& vec, float degrees = PI * 2)
{
	float phi = RandFloat(0, degrees);
	float costheta = RandFloat(-1, 1);

	float theta = acosf(costheta);
	float rs = radius * sinf(theta);
	vec[0] = rs * cosf(phi);
	vec[1] = rs * sinf(phi);
	vec[2] = radius * cosf(theta);
}

static uint32_t Hash(uint32_t a)
{
	a = (a ^ 61) ^ (a >> 16);
	a = a + (a << 3);
	a = a ^ (a >> 4);
	a = a * 0x27d4eb2d;
	a = a ^ (a >> 15);
	return a;
}

}



