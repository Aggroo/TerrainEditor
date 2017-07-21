#pragma once
#include <iostream>
#include <math.h>

namespace CGMath
{
	class vector2D
	{
	public:
		//Konstruktor
		inline vector2D()
		{

			vec[0] = 0;
			vec[1] = 1;

		}

		inline vector2D(int x, int y)
		{
			vec[0] = x;
			vec[1] = y;
		}

		inline vector2D(float x, float y)
		{
			vec[0] = x;
			vec[1] = y;
		}

		inline vector2D(float* arr)
		{
			vec[0] = arr[0];
			vec[1] = arr[1];
		}

		//Dekonstruktor
		inline ~vector2D()
		{

		}

		//vector + vector operators �verlagring
		inline vector2D operator+(vector2D vect)
		{
			vector2D new_vec;

			new_vec[0] = vec[0] + vect[0];
			new_vec[1] = vec[1] + vect[1];

			return new_vec;
		}

		inline vector2D operator-(vector2D& vect)
		{
			vector2D new_vec;

			new_vec[0] = vec[0] - vect[0];
			new_vec[1] = vec[1] - vect[1];

			return new_vec;
		}

		inline vector2D operator*(const float& constant)
		{
			vector2D new_vec;

			new_vec[0] = vec[0] * constant;
			new_vec[1] = vec[1] * constant;

			return new_vec;
		}

		inline vector2D operator/(const float& constant)
		{
			vector2D new_vec;

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
		

		inline vector2D vecMult(float constant)
		{
			vector2D new_vec;

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

		inline vector2D vecNorm()
		{
			vector2D new_vec;

			new_vec[0] = vec[0] / vecLength();
			new_vec[1] = vec[1] / vecLength();
		
			return new_vec;
		}

		inline float dotProd(vector2D vect)
		{
			float result = 0;

			result += vec[0] * vect[0];
			result += vec[1] * vect[1];

			return result;
		}


		inline friend std::ostream& operator<<(std::ostream& stream, const vector2D& vect)
		{
			stream << vect.vec[0] << " ";
			stream << vect.vec[1] << " ";

			std::cout << std::endl;
			return stream;
		}


	private:
		float vec[2];
	
	};
}

typedef CGMath::vector2D vec2;
