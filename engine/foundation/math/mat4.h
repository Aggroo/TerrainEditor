#pragma once
#include <cmath>
#include "vec4.h"
#include "quaternions.h"


namespace Math
{
class mat4
{
public:
	inline mat4()
	{
        matrice[0] = 0; matrice[1] = 0; matrice[2] = 0; matrice[3] = 0;
		matrice[4] = 0; matrice[5] = 0; matrice[6] = 0; matrice[7] = 0;
		matrice[8] = 0;
        matrice[9] = 0;
        matrice[10] = 0;
        matrice[11] = 0;

        matrice[12] = 0;
        matrice[13] = 0;
        matrice[14] = 0;

		matrice[15] = 1;
	}
	
	inline mat4(const float m00, const float m01, const float m02, const float m03,
			const float m10, const float m11, const float m12, const float m13,
			const float m20, const float m21, const float m22, const float m23,
			const float m30, const float m31, const float m32, const float m33)
	{
		matrice[0] = m00;
		matrice[1] = m01;
		matrice[2] = m02;
		matrice[3] = m03;
		
		matrice[4] = m10;
		matrice[5] = m11;
		matrice[6] = m12;
		matrice[7] = m13;
		
		matrice[8] = m20;
		matrice[9] = m21;
		matrice[10] = m22;
		matrice[11] = m23;
		
		matrice[12] = m30;
		matrice[13] = m31;
		matrice[14] = m32;
		matrice[15] = m33;
	}

	inline mat4(const float* val)
	{
		matrice[0] = val[0];
		matrice[1] = val[1];
		matrice[2] = val[2];
		matrice[3] = val[3];

		matrice[4] = val[4];
		matrice[5] = val[5];
		matrice[6] = val[6];
		matrice[7] = val[7];

		matrice[8] = val[8];
		matrice[9] = val[9];
		matrice[10] = val[10];
		matrice[11] = val[11];

		matrice[12] = val[12];
		matrice[13] = val[13];
		matrice[14] = val[14];
		matrice[15] = val[15];
	}
	
	inline ~mat4()
	{

	}

	inline bool operator==(const mat4& rhs) const
	{
		return matrice[0] == rhs[0] && matrice[1] == rhs[1] && matrice[2] == rhs[2] && matrice[3] == rhs[3] &&
			matrice[4] == rhs[4] && matrice[5] == rhs[5] && matrice[6] == rhs[6] && matrice[7] == rhs[7] &&
			matrice[8] == rhs[8] && matrice[9] == rhs[9] && matrice[10] == rhs[10] && matrice[11] == rhs[11] &&
			matrice[12] == rhs[12] && matrice[13] == rhs[13] && matrice[14] == rhs[14] && matrice[15] == rhs[15];
			
	}

	inline bool operator!=(const mat4& rhs) const
	{
		return matrice[0] != rhs[0] || matrice[1] != rhs[1] || matrice[2] != rhs[2] || matrice[3] != rhs[3] ||
			matrice[4] != rhs[4] || matrice[5] != rhs[5] || matrice[6] != rhs[6] || matrice[7] != rhs[7] ||
			matrice[8] != rhs[8] || matrice[9] != rhs[9] || matrice[10] != rhs[10] || matrice[11] != rhs[11] ||
			matrice[12] != rhs[12] || matrice[13] != rhs[13] || matrice[14] != rhs[14] || matrice[15] != rhs[15];

	}

	inline mat4 operator*(const mat4& rhs) const
	{
		mat4 new_matrix;

		new_matrix[0] = matrice[0]*rhs[0]+matrice[1]*rhs[4]+matrice[2]*rhs[8]+matrice[3]*rhs[12];
		new_matrix[1] = matrice[0]*rhs[1]+matrice[1]*rhs[5]+matrice[2]*rhs[9]+matrice[3]*rhs[13];
		new_matrix[2] = matrice[0]*rhs[2]+matrice[1]*rhs[6]+matrice[2]*rhs[10]+matrice[3]*rhs[14];
		new_matrix[3] = matrice[0]*rhs[3]+matrice[1]*rhs[7]+matrice[2]*rhs[11]+matrice[3]*rhs[15];
		
		new_matrix[4] = matrice[4]*rhs[0]+matrice[5]*rhs[4]+matrice[6]*rhs[8]+matrice[7]*rhs[12];
		new_matrix[5] = matrice[4]*rhs[1]+matrice[5]*rhs[5]+matrice[6]*rhs[9]+matrice[7]*rhs[13];
		new_matrix[6] = matrice[4]*rhs[2]+matrice[5]*rhs[6]+matrice[6]*rhs[10]+matrice[7]*rhs[14];
		new_matrix[7] = matrice[4]*rhs[3]+matrice[5]*rhs[7]+matrice[6]*rhs[11]+matrice[7]*rhs[15];
		
		new_matrix[8] = matrice[8]*rhs[0]+matrice[9]*rhs[4]+matrice[10]*rhs[8]+matrice[11]*rhs[12];
		new_matrix[9] = matrice[8]*rhs[1]+matrice[9]*rhs[5]+matrice[10]*rhs[9]+matrice[11]*rhs[13];
		new_matrix[10] = matrice[8]*rhs[2]+matrice[9]*rhs[6]+matrice[10]*rhs[10]+matrice[11]*rhs[14];
		new_matrix[11] = matrice[8]*rhs[3]+matrice[9]*rhs[7]+matrice[10]*rhs[11]+matrice[11]*rhs[15];
		
		new_matrix[12] = matrice[12]*rhs[0]+matrice[13]*rhs[4]+matrice[14]*rhs[8]+matrice[15]*rhs[12];
		new_matrix[13] = matrice[12]*rhs[1]+matrice[13]*rhs[5]+matrice[14]*rhs[9]+matrice[15]*rhs[13];
		new_matrix[14] = matrice[12]*rhs[2]+matrice[13]*rhs[6]+matrice[14]*rhs[10]+matrice[15]*rhs[14];
		new_matrix[15] = matrice[12]*rhs[3]+matrice[13]*rhs[7]+matrice[14]*rhs[11]+matrice[15]*rhs[15];
		
		return new_matrix;
	}


	inline vec4 operator*(const vec4& rhs)
	{
		vec4 new_vec;

		new_vec[0] = matrice[0]*rhs[0] + matrice[1]*rhs[1] + matrice[2]*rhs[2] + matrice[3]*rhs[3];
		new_vec[1] = matrice[4]*rhs[0] + matrice[5]*rhs[1] + matrice[6]*rhs[2] + matrice[7]*rhs[3];
		new_vec[2] = matrice[8]*rhs[0] + matrice[9]*rhs[1] + matrice[10]*rhs[2] + matrice[11]*rhs[3];
		new_vec[3] = matrice[12]*rhs[0] + matrice[13]*rhs[1] + matrice[14]*rhs[2] + matrice[15]*rhs[3];

		return new_vec;
	}

	inline mat4 operator!()
	{
		mat4 new_matrix;

		new_matrix[0] = matrice[0];
		new_matrix[1] = matrice[4];
		new_matrix[2] = matrice[8];
		new_matrix[3] = matrice[12];
		new_matrix[4] = matrice[1];
		new_matrix[5] = matrice[5];
		new_matrix[6] = matrice[9];
		new_matrix[7] = matrice[13];
		new_matrix[8] = matrice[2];
		new_matrix[9] = matrice[6];
		new_matrix[10] = matrice[10];
		new_matrix[11] = matrice[14];
		new_matrix[12] = matrice[3];
		new_matrix[13] = matrice[7];
		new_matrix[14] = matrice[11];
		new_matrix[15] = matrice[15];

		return new_matrix;
	}

	inline float &operator[](int pos)
	{
		return matrice[pos];
	}

	inline float operator[](int pos) const
	{
		return matrice[pos];
	}

	inline static mat4 identity()
	{
		return mat4(1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1);
	}

	inline static mat4 inverse(Math::mat4& matrice)
	{
		mat4 new_matrix, tmp;

		tmp[0] = matrice[5] * matrice[10] * matrice[15] -
				 matrice[5] * matrice[11] * matrice[14] -
				 matrice[9] * matrice[6] * matrice[15] +
				 matrice[9] * matrice[7] * matrice[14] +
				 matrice[13] * matrice[6] * matrice[11] -
				 matrice[13] * matrice[7] * matrice[10];
		
		tmp[1] = -matrice[1] * matrice[10] * matrice[15] +
				 matrice[1] * matrice[11] * matrice[14] +
				 matrice[9] * matrice[2] * matrice[15] -
				 matrice[9] * matrice[3] * matrice[14] -
				 matrice[13] * matrice[2] * matrice[11] +
				 matrice[13] * matrice[3] * matrice[10];
		
		tmp[2] = matrice[1] * matrice[6] * matrice[15] -
				 matrice[1] * matrice[7] * matrice[14] -
				 matrice[5] * matrice[2] * matrice[15] +
				 matrice[5] * matrice[3] * matrice[14] +
				 matrice[13] * matrice[2] * matrice[7] -
				 matrice[13] * matrice[3] * matrice[6];
		
		tmp[3] = -matrice[1] * matrice[6] * matrice[11] +
				 matrice[1] * matrice[7] * matrice[10] +
				 matrice[5] * matrice[2] * matrice[11] -
				 matrice[5] * matrice[3] * matrice[10] -
				 matrice[9] * matrice[2] * matrice[7] +
				 matrice[9] * matrice[3] * matrice[6];
		
		tmp[4] = -matrice[4] * matrice[10] * matrice[15] +
				 matrice[4] * matrice[11] * matrice[14] +
				 matrice[8] * matrice[6] * matrice[15] -
				 matrice[8] * matrice[7] * matrice[14] -
				 matrice[12] * matrice[6] * matrice[11] +
				 matrice[12] * matrice[7] * matrice[10];
		
		tmp[5] = matrice[0] * matrice[10] * matrice[15] -
				 matrice[0] * matrice[11] * matrice[14] -
				 matrice[8] * matrice[2] * matrice[15] +
				 matrice[8] * matrice[3] * matrice[14] +
				 matrice[12] * matrice[2] * matrice[11] -
				 matrice[12] * matrice[3] * matrice[10];
		
		tmp[6] = -matrice[0] * matrice[6] * matrice[15] +
				 matrice[0] * matrice[7] * matrice[14] +
				 matrice[4] * matrice[2] * matrice[15] -
				 matrice[4] * matrice[3] * matrice[14] -
				 matrice[12] * matrice[2] * matrice[7] +
				 matrice[12] * matrice[3] * matrice[6];
		
		tmp[7] = matrice[0] * matrice[6] * matrice[11] -
				 matrice[0] * matrice[7] * matrice[10] -
				 matrice[4] * matrice[2] * matrice[11] +
				 matrice[4] * matrice[3] * matrice[10] +
				 matrice[8] * matrice[2] * matrice[7] -
				 matrice[8] * matrice[3] * matrice[6];
		
		tmp[8] = matrice[4] * matrice[9] * matrice[15] -
				 matrice[4] * matrice[11] * matrice[13] -
				 matrice[8] * matrice[5] * matrice[15] +
				 matrice[8] * matrice[7] * matrice[13] +
				 matrice[12] * matrice[5] * matrice[11] -
				 matrice[12] * matrice[7] * matrice[9];
		
		tmp[9] = -matrice[0] * matrice[9] * matrice[15] +
				 matrice[0] * matrice[11] * matrice[13] +
				 matrice[8] * matrice[1] * matrice[15] -
				 matrice[8] * matrice[3] * matrice[13] -
				 matrice[12] * matrice[1] * matrice[11] +
				 matrice[12] * matrice[3] * matrice[9];
		
		tmp[10] = matrice[0] * matrice[5] * matrice[15] -
				 matrice[0] * matrice[7] * matrice[13] -
				 matrice[4] * matrice[1] * matrice[15] +
				 matrice[4] * matrice[3] * matrice[13] +
				 matrice[12] * matrice[1] * matrice[7] -
				 matrice[12] * matrice[3] * matrice[5];
		
		tmp[11] = -matrice[0] * matrice[5] * matrice[11] +
				 matrice[0] * matrice[7] * matrice[9] +
				 matrice[4] * matrice[1] * matrice[11] -
				 matrice[4] * matrice[3] * matrice[9] -
				 matrice[8] * matrice[1] * matrice[7] +
				 matrice[8] * matrice[3] * matrice[5];
		
		tmp[12] = -matrice[4] * matrice[9] * matrice[14] +
				 matrice[4] * matrice[10] * matrice[13] +
				 matrice[8] * matrice[5] * matrice[14] -
				 matrice[8] * matrice[6] * matrice[13] -
				 matrice[12] * matrice[5] * matrice[10] +
				 matrice[12] * matrice[6] * matrice[9];
		
		tmp[13] = matrice[0] * matrice[9] * matrice[14] -
				 matrice[0] * matrice[10] * matrice[13] -
				 matrice[8] * matrice[1] * matrice[14] +
				 matrice[8] * matrice[2] * matrice[13] +
				 matrice[12] * matrice[1] * matrice[10] -
				 matrice[12] * matrice[2] * matrice[9];
		
		tmp[14] = -matrice[0] * matrice[5] * matrice[14] +
				matrice[0] * matrice[6] * matrice[13] +
				 matrice[4] * matrice[1] * matrice[14] -
				 matrice[4] * matrice[2] * matrice[13] -
				 matrice[12] * matrice[1] * matrice[6] +
				matrice[12] * matrice[2] * matrice[5];
		
		tmp[15] = matrice[0] * matrice[5] * matrice[10] -
				 matrice[0] * matrice[6] * matrice[9] -
				 matrice[4] * matrice[1] * matrice[10] +
				 matrice[4] * matrice[2] * matrice[9] +
				 matrice[8] * matrice[1] * matrice[6] -
				 matrice[8] * matrice[2] * matrice[5];

		// check determinant if it is 0
		const float determinant = matrice[0] * tmp[0] + matrice[1] * tmp[4] + matrice[2] * tmp[8] + matrice[3] * tmp[12];
		if (determinant == 0)
		{
			return identity(); // cannot inverse, make it identity matrix
		}

		// divide by the determinant
		const float invDeterminant = 1.0f / determinant;
		
		for(int i = 0; i < 16; i++)
		{
			new_matrix[i] = invDeterminant * tmp[i];
		}

		return new_matrix;
	}
	inline static mat4 Scale(const float size)
	{
		mat4 scaleMatrix;

		scaleMatrix[0] = size; scaleMatrix[5] = size; scaleMatrix[10] = size;

		return scaleMatrix;
	}

	inline static mat4 Scale(const float sX, const float sY, const float sZ)
	{
		mat4 scaleMatrix;

		scaleMatrix[0] = sX; scaleMatrix[5] = sY; scaleMatrix[10] = sZ;

		return scaleMatrix;
	}
	
	inline static mat4 Scale(const vec4& vec)
	{
		mat4 scaleMatrix;

		scaleMatrix[0] = vec.x(); scaleMatrix[5] = vec.y(); scaleMatrix[10] = vec.x();

		return scaleMatrix;
	}

	inline static mat4 rotX(const float angle)
	{
		mat4 rotMatrix;
		const auto c = cos(angle);
		const auto s = sin(angle);

		rotMatrix[0] = 1; rotMatrix[1] = 0; rotMatrix[2] = 0;
		rotMatrix[4] = 0; rotMatrix[5] = c; rotMatrix[6] = -s;
		rotMatrix[8] = 0; rotMatrix[9] = s; rotMatrix[10] = c;

		return rotMatrix;

	}

	inline static mat4 rotY(const float angle)
	{
		mat4 rotMatrix;
		const auto c = cos(angle);
		const auto s = sin(angle);

		rotMatrix[0] = c; rotMatrix[1] = 0; rotMatrix[2] = s;
		rotMatrix[4] = 0; rotMatrix[5] = 1; rotMatrix[6] = 0;
		rotMatrix[8] = -s; rotMatrix[9] = 0; rotMatrix[10] = c;

		return rotMatrix;
	}

	inline static mat4 rotZ(const float angle)
	{
		mat4 rotMatrix;
		const auto c = cos(angle);
		const auto s = sin(angle);

		rotMatrix[0] = c; rotMatrix[1] = -s; rotMatrix[2] = 0;
		rotMatrix[4] = s; rotMatrix[5] = c; rotMatrix[6] = 0;
		rotMatrix[8] = 0; rotMatrix[9] = 0; rotMatrix[10] = 1;

		return rotMatrix;
	}

	inline static mat4 vecRot(const float angle, const float vecX, const float vecY, const float vecZ)
	{
		vec4 v(vecX, vecY, vecZ);
		mat4 rotMatrix;

		const auto c = cos(angle);
		const auto s = sin(angle);

		rotMatrix[0] = v[0] * v[0] + (1 - v[0] * v[0])*c;
		rotMatrix[1] = v[0] * v[1] * (1 - c) - v[0] * s;
		rotMatrix[2] = v[0] * v[2] * (1 - c) + v[1] * s;
		rotMatrix[4] = v[0] * v[1] * (1 - c) + v[2] * s;
		rotMatrix[5] = v[2] * v[1] + (1 - v[1] * v[1])*c;
		rotMatrix[6] = v[0] * v[2] * (1 - c) - v[0] * s;
		rotMatrix[8] = v[0] * v[2] * (1 - c) - v[1] * s;
		rotMatrix[9] = v[1] * v[2] * (1 - c) + v[0] * s;
		rotMatrix[10] = v[2] * v[2] + (1 - v[2] * v[2])*c;

		return rotMatrix;
	}

	inline static mat4 vecRot(const float angle, const vec4& v)
	{
		mat4 rotMatrix;

		const auto c = cos(angle);
		const auto s = sin(angle);

		rotMatrix[0] = v[0]*v[0] + (1 - v[0]*v[0])*c;
		rotMatrix[1] = v[0]*v[1] * (1 - c) - v[0] * s;
		rotMatrix[2] = v[0]*v[2] * (1 - c) + v[1] * s;
		rotMatrix[4] = v[0]*v[1] * (1 - c) + v[2] * s;
		rotMatrix[5] = v[2]*v[1] + (1 - v[1] *v[1])*c;
		rotMatrix[6] = v[0]*v[2] * (1 - c) - v[0] * s;
		rotMatrix[8] = v[0]*v[2] * (1 - c) - v[1] * s;
		rotMatrix[9] = v[1]*v[2] * (1 - c) + v[0] * s;
		rotMatrix[10] = v[2] *v[2] + (1 - v[2] *v[2])*c;

		return rotMatrix;
	}

	inline float* getPointer()
	{
		
		return &matrice[0];
	}

	inline static mat4 translationMatrix(const vec4& vec)
	{
		mat4 transMatrix;
		transMatrix[0] = 1;
		transMatrix[3] = vec.x();
		transMatrix[5] = 1;
		transMatrix[7] = vec.y();
		transMatrix[10] = 1;
		transMatrix[11] = vec.z();

		return transMatrix;
	}

	inline static mat4 perspectiveMatrix(const float fovy, const float aspect, const float zn, const float zf)
	{
		mat4 fovMatrix;
		const float halfFov = 0.5f * fovy;
		const float sinfov = sinf(halfFov);
		const float cosfov = cosf(halfFov);

		const float height = cosfov / sinfov;
		const float width = height / aspect;

		const float dist = zf / (zn - zf);
		fovMatrix[0] = width;
		fovMatrix[5] = height;
		fovMatrix[10] = dist;
		fovMatrix[11] = dist*zn;
		fovMatrix[14] = -1;
		fovMatrix[15] = 0;
		

		return fovMatrix;
	}

	inline static mat4 LookAt(const vec4& eye, const vec4& target, const vec4& up)
	{
		auto zaxis = vec4::Normalize(eye - target);    // The "forward" vector.
		auto xaxis = vec4::Normalize(vec4::Cross(up, zaxis));// The "right" vector.
		auto yaxis = vec4::Cross(zaxis, xaxis);

		// Create a 4x4 view matrix from the right, up, forward and eye position vectors
		mat4 viewMatrix(xaxis.x(), xaxis.y(), xaxis.z(), -vec4::Dot(xaxis, eye),
						yaxis.x(), yaxis.y(), yaxis.z(), -vec4::Dot(yaxis, eye),
						zaxis.x(), zaxis.y(), zaxis.z(), -vec4::Dot(zaxis, eye),
						0, 0, 0, 1);

		return viewMatrix;
	}

	inline vec4 GetXAxis()
	{
		vec4 vec(matrice[0],matrice[4],matrice[8]);
		return vec;
	}

	inline vec4 GetYAxis()
	{
		vec4 vec(matrice[1],matrice[5],matrice[9]);
		return vec;
	}
	inline vec4 GetZAxis()
	{
		vec4 vec(matrice[2],matrice[6],matrice[10]);
		return vec;
	}
	
	inline static mat4 Transpose(mat4 m)
	{
		mat4 mat;

		mat[0] = m[0];
		mat[1] = m[4];
		mat[2] = m[8];
		mat[3] = m[12]; 
		mat[4] = m[1];
		mat[5] = m[5];
		mat[6] = m[9];
		mat[7] = m[13];
		mat[8] = m[2];
		mat[9] = m[6];
		mat[10] = m[10];
		mat[11] = m[14];
		mat[12] = m[3];
		mat[13] = m[7];
		mat[14] = m[11];
		mat[15] = m[15];
		
		return mat;
		
	}

	inline vec4 GetPosition()
	{
		vec4 vec;

		vec[0] = matrice[3];
		vec[1] = matrice[7];
		vec[2] = matrice[11];
		vec[3] = matrice[15];
		return vec;
	}

	inline void SetPosition(const vec4& vec)
	{
		matrice[3] = vec[0];
		matrice[7] = vec[1];
		matrice[11] = vec[2];
		matrice[15] = vec[3];

	}
	
	inline static mat4 RotationQuaternion(const quaternions& q)
	{
		float xx = q.X() * q.X();
		float xy = q.X() * q.Y();
		float xz = q.X() * q.Z();
		float xw = q.X() * q.W();
		
		float yy = q.Y() * q.Y();
		float yz = q.Y() * q.Z();
		float yw = q.Y() * q.W();
		
		float zz = q.Z() * q.Z();
		float zw = q.Z() * q.W();
		
		float m00 = 1 - 2 * ( yy + zz );
		float m01 =     2 * ( xy - zw );
		float m02 =     2 * ( xz + yw );
		
		float m10 =     2 * ( xy + zw );
		float m11 = 1 - 2 * ( xx + zz );
		float m12 =     2 * ( yz - xw );
		
		float m20 =     2 * ( xz - yw );
		float m21 =     2 * ( yz + xw );
		float m22 = 1 - 2 * ( xx + yy );
		
		float m03 = 0.0f, m13 = 0.0f, m23 = 0.0f, m30 = 0.0f, m31 = 0.0f, m32 = 0.0f;
		float m33 = 1.0f;
		
		return mat4(m00, m01, m02, m03
				,m10, m11, m12, m13
				,m20, m21, m22, m23
				,m30, m31, m32, m33);
	}

private:
	float matrice[16];
};

}



