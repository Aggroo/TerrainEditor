#pragma once
#include <math.h>

namespace Math
{
	class vec2
	{
	public:
		//Konstruktor
		inline vec2()
		{

			vec[0] = 0;
			vec[1] = 1;

		}

		inline vec2(int x, int y)
		{
			vec[0] = x;
			vec[1] = y;
		}

		inline vec2(float x, float y)
		{
			vec[0] = x;
			vec[1] = y;
		}

		inline vec2(float v)
		{
			vec[0] = v;
			vec[1] = v;
		}

		inline vec2(float* arr)
		{
			vec[0] = arr[0];
			vec[1] = arr[1];
		}

		//Dekonstruktor
		inline ~vec2()
		{

		}

		//vector + vector operators �verlagring
		inline vec2 operator+(vec2 vect)
		{
			vec2 new_vec;

			new_vec[0] = vec[0] + vect[0];
			new_vec[1] = vec[1] + vect[1];

			return new_vec;
		}

		inline vec2 operator-(vec2& vect)
		{
			vec2 new_vec;

			new_vec[0] = vec[0] - vect[0];
			new_vec[1] = vec[1] - vect[1];

			return new_vec;
		}

		inline vec2 operator*(const float& constant)
		{
			vec2 new_vec;

			new_vec[0] = vec[0] * constant;
			new_vec[1] = vec[1] * constant;

			return new_vec;
		}

		inline vec2 operator/(const float& constant)
		{
			vec2 new_vec;

			new_vec[0] = vec[0] / constant;
			new_vec[1] = vec[1] / constant;

			return new_vec;
		}

		inline float &operator[](int pos)
		{
			return vec[pos];
		}

		inline float& x()
		{
			return vec[0];
		}
		inline float& y()
		{
			return vec[1];
		}
		

		inline vec2 vecMult(float constant)
		{
			vec2 new_vec;

			new_vec[0] = vec[0] * constant;
			new_vec[1] = vec[1] * constant;

			return new_vec;
		}

		inline float vecLength()
		{
			float length;
			float tmp = 0;

			tmp += pow(vec[0], 2);
			tmp += pow(vec[1], 2);

			length = sqrt(tmp);

			return length;
		}

		inline vec2 vecNorm()
		{
			vec2 new_vec;

			new_vec[0] = vec[0] / vecLength();
			new_vec[1] = vec[1] / vecLength();
		
			return new_vec;
		}

		inline float dotProd(vec2 vect)
		{
			float result = 0;

			result += vec[0] * vect[0];
			result += vec[1] * vect[1];

			return result;
		}


	private:
		float vec[2];
	
	};
}
