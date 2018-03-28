#pragma once
#include <math.h>
#include "vec3.h"

namespace Math
{
	class vec4
	{
	public:
		inline vec4()
		{

			vec[0] = 0;
			vec[1] = 0;
			vec[2] = 0;

			vec[3] = 1;
		}
		inline vec4(int x, int y, int z)
		{
			vec[0] = x;
			vec[1] = y;
			vec[2] = z;
			vec[3] = 1;
		}
		inline vec4(float x, float y, float z)
		{
			vec[0] = x;
			vec[1] = y;
			vec[2] = z;
			vec[3] = 1;
		}

		inline vec4(vec3 v)
		{
			vec[0] = v.x();
			vec[1] = v.y();
			vec[2] = v.z();
			vec[3] = 1;
		}
		inline vec4(float x, float y, float z , float w)
		{
			vec[0] = x;
			vec[1] = y;
			vec[2] = z;
			vec[3] = w;
		}
		inline ~vec4()
		{

		}

		inline vec4 operator+(vec4 vect)
		{
			vec4 new_vec;

			new_vec[0] = vec[0] + vect[0];
			new_vec[1] = vec[1] + vect[1];
			new_vec[2] = vec[2] + vect[2];

			return new_vec;
		}

		inline void operator+=(vec4 vect)
		{
			vec[0] += vect[0];
			vec[1] += vect[1];
			vec[2] += vect[2];
		}

        inline void operator=(vec3 vect)
        {
            vec[0] = vect[0];
            vec[1] = vect[1];
            vec[2] = vect[2];
        }

		inline vec4 operator-(const vec4& vect) const
		{
			vec4 new_vec;

			new_vec[0] = vec[0] - vect.vec[0];
			new_vec[1] = vec[1] - vect.vec[1];
			new_vec[2] = vec[2] - vect.vec[2];

			return new_vec;
		}

        inline vec4 operator-(const float& constant) const
        {
            vec4 new_vec;

            new_vec[0] = vec[0] - constant;
            new_vec[1] = vec[1] - constant;
            new_vec[2] = vec[2] - constant;

            return new_vec;
        }

		inline vec4 operator*(const vec4& vect) const
		{
			vec4 new_vec;

			new_vec[0] = vec[0] * vect.vec[0];
			new_vec[1] = vec[1] * vect.vec[1];
			new_vec[2] = vec[2] * vect.vec[2];

			return new_vec;
		}

		inline vec4 operator*(int& constant)
		{
			vec4 new_vec;

			new_vec[0] = vec[0] * constant;
			new_vec[1] = vec[1] * constant;
			new_vec[2] = vec[2] * constant;

			return new_vec;
		}

		inline vec4 operator*(float& constant)
		{
			vec4 new_vec;

			new_vec[0] = vec[0] * constant;
			new_vec[1] = vec[1] * constant;
			new_vec[2] = vec[2] * constant;

			return new_vec;
		}

		inline bool operator==(const vec4 &other) const
		{
			return (vec[0] == other.vec[0] && vec[1] == other.vec[1] && vec[2] == other.vec[2]);
		}

		inline vec4 operator*(const float& constant) const
		{
			vec4 new_vec;

			new_vec[0] = vec[0] * constant;
			new_vec[1] = vec[1] * constant;
			new_vec[2] = vec[2] * constant;

			return new_vec;
		}

		inline vec4 operator/(vec4& vect)
		{
			vec4 new_vec;

			new_vec[0] = vec[0] / vect[0];
			new_vec[1] = vec[1] / vect[1];
			new_vec[2] = vec[2] / vect[2];

			return new_vec;
		}

		inline vec4 operator/(const float& constant) const
		{
			vec4 new_vec;

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

		inline const float &operator[](int pos) const
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

		inline vec3 ToVec3()
		{
			vec3 new_vec;

			new_vec[0] = vec[0];
			new_vec[1] = vec[1];
			new_vec[2] = vec[2];

			return new_vec;
		}

		inline vec4 Negative() const
		{
			return vec4(-vec[0],-vec[1],-vec[2]);
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

		inline static vec4 Normalize(vec4 vec)
		{
			vec4 new_vec;
			float norm = 1/vec.vecLength();

			new_vec[0] = vec[0] * norm;
			new_vec[1] = vec[1] * norm;
			new_vec[2] = vec[2] * norm;

			return new_vec;
		}

		inline static float Dot(vec4 vec, vec4 vect)
		{
			float result = 0;

			result += vec[0] * vect[0];
			result += vec[1] * vect[1];
			result += vec[2] * vect[2];

			return result;
		}

		inline static vec4 Cross(vec4 vec, vec4 vect)
		{
			vec4 new_vec;

			new_vec[0] = vec[1] * vect[2] - vec[2] * vect[1];
			new_vec[1] = vec[2] * vect[0] - vec[0] * vect[2];
			new_vec[2] = vec[0] * vect[1] - vec[1] * vect[0];
		

			return new_vec;
		}


	private:
		float vec[4];
	
	};
	
	typedef Math::vec4 vec4;
}

