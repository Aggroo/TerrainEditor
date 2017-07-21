#pragma once
#include <iostream>
#include <math.h>
#include "vector3D.h"

namespace CGMath
{
	class vector4D
	{
	public:
		inline vector4D()
		{

			vec[0] = 0;
			vec[1] = 0;
			vec[2] = 0;

			vec[3] = 1;
		}
		inline vector4D(int x, int y, int z)
		{
			vec[0] = x;
			vec[1] = y;
			vec[2] = z;
			vec[3] = 1;
		}
		inline vector4D(float x, float y, float z)
		{
			vec[0] = x;
			vec[1] = y;
			vec[2] = z;
			vec[3] = 1;
		}

		inline vector4D(vector3D v)
		{
			vec[0] = v.x();
			vec[1] = v.y();
			vec[2] = v.z();
			vec[3] = 1;
		}
		inline vector4D(float x, float y, float z , float w)
		{
			vec[0] = x;
			vec[1] = y;
			vec[2] = z;
			vec[3] = w;
		}
		inline ~vector4D()
		{

		}

		inline vector4D operator+(vector4D vect)
		{
			vector4D new_vec;

			new_vec[0] = vec[0] + vect[0];
			new_vec[1] = vec[1] + vect[1];
			new_vec[2] = vec[2] + vect[2];

			return new_vec;
		}

		inline void operator+=(vector4D vect)
		{
			vec[0] += vect[0];
			vec[1] += vect[1];
			vec[2] += vect[2];
		}

        inline void operator=(vector3D vect)
        {
            vec[0] = vect[0];
            vec[1] = vect[1];
            vec[2] = vect[2];
        }

		inline vector4D operator-(const vector4D& vect) const
		{
			vector4D new_vec;

			new_vec[0] = vec[0] - vect.vec[0];
			new_vec[1] = vec[1] - vect.vec[1];
			new_vec[2] = vec[2] - vect.vec[2];

			return new_vec;
		}

        inline vector4D operator-(const float& constant) const
        {
            vector4D new_vec;

            new_vec[0] = vec[0] - constant;
            new_vec[1] = vec[1] - constant;
            new_vec[2] = vec[2] - constant;

            return new_vec;
        }

		inline vector4D operator*(const vector4D& vect) const
		{
			vector4D new_vec;

			new_vec[0] = vec[0] * vect.vec[0];
			new_vec[1] = vec[1] * vect.vec[1];
			new_vec[2] = vec[2] * vect.vec[2];

			return new_vec;
		}

		inline vector4D operator*(int& constant)
		{
			vector4D new_vec;

			new_vec[0] = vec[0] * constant;
			new_vec[1] = vec[1] * constant;
			new_vec[2] = vec[2] * constant;

			return new_vec;
		}

		inline vector4D operator*(float& constant)
		{
			vector4D new_vec;

			new_vec[0] = vec[0] * constant;
			new_vec[1] = vec[1] * constant;
			new_vec[2] = vec[2] * constant;

			return new_vec;
		}

		inline bool operator==(const vector4D &other) const
		{
			return (vec[0] == other.vec[0] && vec[1] == other.vec[1] && vec[2] == other.vec[2]);
		}

		inline vector4D operator*(const float& constant) const
		{
			vector4D new_vec;

			new_vec[0] = vec[0] * constant;
			new_vec[1] = vec[1] * constant;
			new_vec[2] = vec[2] * constant;

			return new_vec;
		}

		inline vector4D operator/(vector4D& vect)
		{
			vector4D new_vec;

			new_vec[0] = vec[0] / vect[0];
			new_vec[1] = vec[1] / vect[1];
			new_vec[2] = vec[2] / vect[2];

			return new_vec;
		}

		inline vector4D operator/(const float& constant) const
		{
			vector4D new_vec;

			new_vec[0] = vec[0] / constant;
			new_vec[1] = vec[1] / constant;
			new_vec[2] = vec[2] / constant;

			return new_vec;
		}

        inline bool operator<(const float& constant)
        {
            if(vec[0] < constant)
                if(vec[1] < constant)
                    if(vec[2] < constant)
                        return true;

            return false;
        }

        inline bool operator>(const float& constant) const
        {
            if(vec[0] > constant)
                if(vec[1] > constant)
                    if(vec[2] > constant)
                        return true;

            return false;
        }

		inline float &operator[](int pos)
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

		inline float w() const
		{
			return vec[3];
		}

		inline vector3D ToVec3()
		{
			vector3D new_vec;

			new_vec[0] = vec[0];
			new_vec[1] = vec[1];
			new_vec[2] = vec[2];

			return new_vec;
		}

		inline vector4D Negative() const
		{
			return vector4D(-vec[0],-vec[1],-vec[2]);
		}

		inline float vecLength()
		{
			float length;
			float tmp = 0;

			tmp += vec[0]*vec[0];
			tmp += vec[1]*vec[1];
			tmp += vec[2]*vec[2];

			length = sqrt(tmp);

			return length;
		}

		inline static vector4D Normalize(vector4D vec)
		{
			vector4D new_vec;
			float norm = 1/vec.vecLength();

			new_vec[0] = vec[0] * norm;
			new_vec[1] = vec[1] * norm;
			new_vec[2] = vec[2] * norm;

			return new_vec;
		}

		inline static float Dot(vector4D vec, vector4D vect)
		{
			float result = 0;

			result += vec[0] * vect[0];
			result += vec[1] * vect[1];
			result += vec[2] * vect[2];

			return result;
		}

		inline static vector4D Cross(vector4D vec, vector4D vect)
		{
			vector4D new_vec;

			new_vec[0] = vec[1] * vect[2] - vec[2] * vect[1];
			new_vec[1] = vec[2] * vect[0] - vec[0] * vect[2];
			new_vec[2] = vec[0] * vect[1] - vec[1] * vect[0];
		

			return new_vec;
		}


		inline friend std::ostream& operator<<(std::ostream& stream, const vector4D& vect)
		{
			stream << vect.vec[0] << " ";
			stream << vect.vec[1] << " ";
			stream << vect.vec[2] << " ";

			std::cout << std::endl;
			return stream;
		}


	private:
		float vec[4];
	
	};
	
	typedef CGMath::vector4D vec4;
}

