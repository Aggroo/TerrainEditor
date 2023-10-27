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

		vec[0] = 0.0f;
		vec[1] = 0.0f;
		vec[2] = 0.0f;
		vec[3] = 1.0f;
	}
	inline vec4(const int x, const int y, const int z)
	{
		vec[0] = float(x);
		vec[1] = float(y);
		vec[2] = float(z);
		vec[3] = 1.0f;
	}
	inline vec4(const float x, const float y, const float z)
	{
		vec[0] = x;
		vec[1] = y;
		vec[2] = z;
		vec[3] = 1.0f;
	}

	inline vec4(vec3 v)
	{
		vec[0] = v.x();
		vec[1] = v.y();
		vec[2] = v.z();
		vec[3] = 1.0f;
	}
	inline vec4(const float x, const  float y, const float z , const float w)
	{
		vec[0] = x;
		vec[1] = y;
		vec[2] = z;
		vec[3] = w;
	}
	inline ~vec4()
	{

	}

	inline vec4 operator+(const vec4& vect) const
	{
		vec4 new_vec;

		new_vec[0] = vec[0] + vect[0];
		new_vec[1] = vec[1] + vect[1];
		new_vec[2] = vec[2] + vect[2];

		return new_vec;
	}

	inline void operator+=(const vec4& vect)
	{
		vec[0] += vect[0];
		vec[1] += vect[1];
		vec[2] += vect[2];
	}

    inline void operator=(const vec3& vect)
    {
        vec[0] = vect[0];
        vec[1] = vect[1];
        vec[2] = vect[2];
    }

	inline vec4 operator-(const vec4& vect) const
	{
		return vec4(vec[0] - vect[0], vec[1] - vect[1], vec[2] - vect[2]);
	}

    inline vec4 operator-(const float& constant) const
    {
        return vec4(vec[0] - constant, vec[1] - constant, vec[2] - constant);
    }

	inline vec4 operator*(const vec4& vect) const
	{
		return vec4(vec[0] * vect[0], vec[1] * vect[1], vec[2] * vect[2]);
	}

	inline vec4 operator*(int& constant)
	{
		return vec4(vec[0] * constant, vec[1] * constant, vec[2] * constant);
	}

	inline vec4 operator*(float& constant)
	{
		return vec4(vec[0] * constant, vec[1] * constant, vec[2] * constant);
	}

	inline bool operator==(const vec4 &other) const
	{
		return (vec[0] == other.vec[0] && vec[1] == other.vec[1] && vec[2] == other.vec[2]);
	}

	inline vec4 operator*(const float& constant) const
	{
		return vec4(vec[0] * constant, vec[1] * constant, vec[2] * constant);
	}

	inline vec4 operator/(vec4& vect)
	{
		return vec4(vec[0] / vect[0], vec[1] / vect[1], vec[2] / vect[2]);
	}

	inline vec4 operator/(const float& constant) const
	{
		return vec4(vec[0] / constant, vec[1] / constant, vec[2] / constant);
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

	inline float &operator[](const int pos)
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

	inline float w() const
	{
		return vec[3];
	}

	inline vec3 ToVec3()
	{
		return vec3(vec[0], vec[1], vec[2]);
	}

	inline vec4 Negative() const
	{
		return vec4(-vec[0],-vec[1],-vec[2]);
	}

	inline float Length() const
	{
		float length;
		float tmp = vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];

		length = sqrt(tmp);

		return length;
	}

	inline static vec4 Normalize(const vec4& vec)
	{
		const float norm = 1/vec.Length();
		return vec4(vec[0] * norm, vec[1] * norm, vec[2] * norm);
	}

	inline static vec4 Orthogonal(const vec4& v)
	{
		if (0.0 != v.x())
		{
			return Math::vec4((-v.y() - v.z()) / v.x(), 1.0f, 1.0f, 0.0f);
		}
		else if (0.0 != v.y())
		{
			return Math::vec4(1.0, (-v.x() - v.z()) / v.y(), 1.0f, 0.0f);
		}
		else if (0.0 != v.z())
		{
			return Math::vec4(1.0f, 1.0f, (-v.x() - v.y()) / v.z(), 0.0f);
		}
		else
		{
			return Math::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		}
	}

	inline static float Dot(const vec4& vec, const vec4& vect)
	{
		return (vec[0] * vect[0] + vec[1] * vect[1] + vec[2] * vect[2]);
	}

	inline static vec4 Cross(const vec4& vec, const vec4& vect)
	{
		return vec4(vec[1] * vect[2] - vec[2] * vect[1], vec[2] * vect[0] - vec[0] * vect[2], vec[0] * vect[1] - vec[1] * vect[0]);
	}

	inline float* ToFloat4()
	{
		return vec;
	}

private:
	float vec[4];

};

typedef Math::vec4 vec4;
}

