#pragma once
#include <iostream>
#include <math.h>


namespace CGMath
{
	class vector3D
	{
	public:
		//Konstruktor
		inline vector3D()
		{
			vec[0] = 0.f;
			vec[1] = 0.f;
			vec[2] = 0.f;
		}
		inline vector3D(int x, int y, int z)
		{
			vec[0] = x;
			vec[1] = y;
			vec[2] = z;
		}

		inline vector3D(float x, float y, float z)
		{
			vec[0] = x;
			vec[1] = y;
			vec[2] = z;
		}

		inline vector3D(float* arr)
		{
			vec[0] = arr[0];
			vec[1] = arr[1];
			vec[2] = arr[2];
		}

		//Dekonstruktor
		inline ~vector3D()
		{

		}

		//Vector + vector operators �verlagring
		inline vector3D operator+(const vector3D& vect) const
		{
			vector3D new_vec;

			new_vec[0] = vec[0] + vect[0];
			new_vec[1] = vec[1] + vect[1];
			new_vec[2] = vec[2] + vect[2];
			
			return new_vec;
		}
		inline vector3D operator-(const vector3D& vect) const
		{
			vector3D new_vec;

			new_vec[0] = vec[0] - vect[0];
			new_vec[1] = vec[1] - vect[1];
			new_vec[2] = vec[2] - vect[2];
			

			return new_vec;
		}

		inline vector3D operator*(float constant) const
		{
			vector3D new_vec;


			new_vec[0] = vec[0] * constant;
			new_vec[1] = vec[1] * constant;
			new_vec[2] = vec[2] * constant;

			return new_vec;
		}

		inline vector3D operator/(float constant) const
		{
			vector3D new_vec; 

			new_vec[0] = vec[0] / constant;
			new_vec[1] = vec[1] / constant;
			new_vec[2] = vec[2] / constant;

			return new_vec;
		}

		//Vector += vector operators overload
		inline vector3D operator+=(const vector3D& vect)
		{
			this->vec[0] = vec[0] + vect[0];
			this->vec[1] = vec[1] + vect[1];
			this->vec[2] = vec[2] + vect[2];

			return *this;
		}
		
		inline float &operator[](int pos)
		{
			return vec[pos];
		}

		inline float operator[](int pos) const
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

		inline vector3D Negative() const
		{
			return vector3D(-vec[0],-vec[1],-vec[2]);
		}

		inline vector3D vecMult(const float& constant)
		{
			vector3D new_vec;

			for (int i = 0; i < 3; i++)
			{
				new_vec[i] = vec[i] * constant;
			}
			return new_vec;
		}

		inline float vecLength()
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

		inline static vector3D Normalize(vector3D v)
		{
			vector3D new_vec;
			float norm = 1 / v.vecLength();
			
			new_vec[0] = v[0] * norm;
			new_vec[1] = v[1] * norm;
			new_vec[2] = v[2] * norm;
		
			return new_vec;
		}

		inline static float Dot(vector3D vec,vector3D vect)
		{
			float result = 0;

			result += vec[0] * vect[0];
			result += vec[1] * vect[1];
			result += vec[2] * vect[2];

			return result;
		}

		inline static vector3D Cross(vector3D vec, vector3D vect)
		{
			vector3D new_vec;

			new_vec[0] = vec[1] * vect[2] - vec[2] * vect[1];
			new_vec[1] = vec[2] * vect[0] - vec[0] * vect[2];
			new_vec[2] = vec[0] * vect[1] - vec[1] * vect[0];

			return new_vec;
		}

		inline friend std::ostream& operator<<(std::ostream& stream, const vector3D& vect)
		{
			for (int y = 0; y < 3; y++)
			{
				stream << vect.vec[y] << " ";

			}
			std::cout << std::endl;
			return stream;
		}

	private:
		float vec[3];
	
	};
	typedef CGMath::vector3D vec3;
}


