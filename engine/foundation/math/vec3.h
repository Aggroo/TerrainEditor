#pragma once
#include <math.h>


namespace Math
{
	class vec3
	{
	public:
		//Konstruktor
		inline vec3()
		{
			vec[0] = 0.0f;
			vec[1] = 0.0f;
			vec[2] = 0.0f;
		}
		inline vec3(const int x, const int y, const int z)
		{
			vec[0] = float(x);
			vec[1] = float(y);
			vec[2] = float(z);
		}

		inline vec3(const float x, const float y, const float z)
		{
			vec[0] = x;
			vec[1] = y;
			vec[2] = z;
		}

		inline vec3(float* arr)
		{
			vec[0] = arr[0];
			vec[1] = arr[1];
			vec[2] = arr[2];
		}

		//Dekonstruktor
		inline ~vec3()
		{

		}

		//Vector + vector operators �verlagring
		inline vec3 operator+(const vec3& vect) const
		{
			vec3 new_vec;

			new_vec[0] = vec[0] + vect[0];
			new_vec[1] = vec[1] + vect[1];
			new_vec[2] = vec[2] + vect[2];
			
			return new_vec;
		}
		inline vec3 operator-(const vec3& vect) const
		{
			vec3 new_vec;

			new_vec[0] = vec[0] - vect[0];
			new_vec[1] = vec[1] - vect[1];
			new_vec[2] = vec[2] - vect[2];
			

			return new_vec;
		}

		inline vec3 operator*(const float constant) const
		{
			vec3 new_vec;


			new_vec[0] = vec[0] * constant;
			new_vec[1] = vec[1] * constant;
			new_vec[2] = vec[2] * constant;

			return new_vec;
		}

		inline vec3 operator/(const float constant) const
		{
			vec3 new_vec; 

			new_vec[0] = vec[0] / constant;
			new_vec[1] = vec[1] / constant;
			new_vec[2] = vec[2] / constant;

			return new_vec;
		}

		//Vector += vector operators overload
		inline vec3 operator+=(const vec3& vect)
		{
			this->vec[0] = vec[0] + vect[0];
			this->vec[1] = vec[1] + vect[1];
			this->vec[2] = vec[2] + vect[2];

			return *this;
		}
		
		inline float& operator[](const int pos)
		{
			return vec[pos];
		}

		inline const float &operator[](const int pos) const
		{
			return vec[pos];
		}

		inline float x() const
		{
			return vec[0];
		}
		inline float y() const
		{
			return vec[1];
		}
		inline float z() const
		{
			return vec[2];
		}
		
		inline float& x() 
		{
			return vec[0];
		}
		inline float& y() 
		{
			return vec[1];
		}
		inline float& z() 
		{
			return vec[2];
		}

		inline vec3 Negative() const
		{
			return vec3(-vec[0],-vec[1],-vec[2]);
		}

		inline vec3 vecMult(const float constant)
		{
			vec3 new_vec;

			for (int i = 0; i < 3; i++)
			{
				new_vec[i] = vec[i] * constant;
			}
			return new_vec;
		}

		inline float vecLength() const
		{
			float length;
			float tmp = 0;

			for (int i = 0; i < 3; i++)
			{
				tmp += pow(vec[i], 2);
			}

			length = sqrt(tmp);

			return length;
		}

		inline static vec3 Normalize(const vec3& v)
		{
			vec3 new_vec;
			float norm = 1 / v.vecLength();
			
			new_vec[0] = v[0] * norm;
			new_vec[1] = v[1] * norm;
			new_vec[2] = v[2] * norm;
		
			return new_vec;
		}

		inline static float Dot(const vec3& vec, const vec3& vect)
		{
			float result = 0;

			result += vec[0] * vect[0];
			result += vec[1] * vect[1];
			result += vec[2] * vect[2];

			return result;
		}

		inline static vec3 Cross(const vec3& vec, const vec3& vect)
		{
			vec3 new_vec;

			new_vec[0] = vec[1] * vect[2] - vec[2] * vect[1];
			new_vec[1] = vec[2] * vect[0] - vec[0] * vect[2];
			new_vec[2] = vec[0] * vect[1] - vec[1] * vect[0];

			return new_vec;
		}


	private:
		float vec[3];
	
	};
	typedef Math::vec3 vec3;
}


