#pragma once
#include <math.h>
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
            matrice[0] = 0;
            matrice[1] = 0;
            matrice[2] = 0;
            matrice[3] = 0;

            matrice[4] = 0;
            matrice[5] = 0;
            matrice[6] = 0;
            matrice[7] = 0;

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
		
		inline ~mat4()
		{

		}

		inline mat4 operator*(mat4 rhs) const
		{
			mat4 new_matrix;

			float value = 0;
			for (int y = 0; y < 4; y++)
			{
				for (int x = 0; x < 4; x++)
				{
					value = 0.0;
					for (int k = 0; k < 4; k++)
					{
						value += matrice[y*4 + k] * rhs[k*4 + x];
					}
					new_matrix[y*4 + x] = value;
				}
			}
			return new_matrix;
		}


		inline vec4 operator*(vec4 rhs)
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

			new_matrix[0*4 + 0] = matrice[0*4 + 0];
			new_matrix[0*4 + 1] = matrice[1*4 + 0];
			new_matrix[0*4 + 2] = matrice[2*4 + 0];
			new_matrix[0*4 + 3] = matrice[3*4 + 0];
			new_matrix[1*4 + 0] = matrice[0*4 + 1];
			new_matrix[1*4 + 1] = matrice[1*4 + 1];
			new_matrix[1*4 + 2] = matrice[2*4 + 1];
			new_matrix[1*4 + 3] = matrice[3*4 + 1];
			new_matrix[2*4 + 0] = matrice[0*4 + 2];
			new_matrix[2*4 + 1] = matrice[1*4 + 2];
			new_matrix[2*4 + 2] = matrice[2*4 + 2];
			new_matrix[2*4 + 3] = matrice[3*4 + 2];
			new_matrix[3*4 + 0] = matrice[0*4 + 3];
			new_matrix[3*4 + 1] = matrice[1*4 + 3];
			new_matrix[3*4 + 2] = matrice[2*4 + 3];
			new_matrix[3*4 + 3] = matrice[3*4 + 3];

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
			mat4 identityMatrix;

			identityMatrix[0] = 1;
			identityMatrix[1] = 0;
			identityMatrix[2] = 0;
			identityMatrix[3] = 0;

			identityMatrix[4] = 0;
			identityMatrix[5] = 1;
			identityMatrix[6] = 0;
			identityMatrix[7] = 0;

			identityMatrix[8] = 0;
			identityMatrix[9] = 0;
			identityMatrix[10] = 1;
			identityMatrix[11] = 0;

			identityMatrix[12] = 0;
			identityMatrix[13] = 0;
			identityMatrix[14] = 0;
			identityMatrix[15] = 1;

			return identityMatrix;
		}

		inline static mat4 inverse(Math::mat4& matrice)
		{
			float tmp[16];
			mat4 new_matrix;

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

		inline static mat4 vectorScaling(const float sX, const float sY, const float sZ)
		{
			mat4 scaleMatrix;

			scaleMatrix[0] = sX; scaleMatrix[5] = sY; scaleMatrix[10] = sZ;

			return scaleMatrix;
		}
		
		inline static mat4 vectorScaling(const vec4& vec)
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
			vec4 rotVec;
			mat4 rotMatrix;

			rotVec[0] = vecX;
			rotVec[1] = vecY;
			rotVec[2] = vecZ;

			const auto c = cos(angle);
			const auto s = sin(angle);

			rotMatrix[0] = rotVec.x()*rotVec.x() + (1 - rotVec.x()*rotVec.x())*c;
			rotMatrix[1] = rotVec.x()*rotVec.y() * (1 - c) - rotVec.x() * s;
			rotMatrix[2] = rotVec.x()*rotVec.z() * (1 - c) + rotVec.y() * s;
			rotMatrix[4] = rotVec.x()*rotVec.y() * (1 - c) + rotVec.z() * s;
			rotMatrix[5] = rotVec.y()*rotVec.y() + (1 - rotVec.y()*rotVec.y())*c;
			rotMatrix[6] = rotVec.x()*rotVec.z() * (1 - c) - rotVec.x() * s;
			rotMatrix[8] = rotVec.x()*rotVec.z() * (1 - c) - rotVec.y() * s;
			rotMatrix[9] = rotVec.y()*rotVec.z() * (1 - c) + rotVec.x() * s;
			rotMatrix[10] = rotVec.z()*rotVec.z() + (1 - rotVec.z()*rotVec.z())*c;

			return rotMatrix;
		}

		inline static mat4 vecRot(const float angle, const vec4& rotVec)
		{
			mat4 rotMatrix;

			const auto c = cos(angle);
			const auto s = sin(angle);

			rotMatrix[0] = rotVec.x()*rotVec.x() + (1 - rotVec.x()*rotVec.x())*c;
			rotMatrix[1] = rotVec.x()*rotVec.y() * (1 - c) - rotVec.x() * s;
			rotMatrix[2] = rotVec.x()*rotVec.z() * (1 - c) + rotVec.y() * s;
			rotMatrix[4] = rotVec.x()*rotVec.y() * (1 - c) + rotVec.z() * s;
			rotMatrix[5] = rotVec.y()*rotVec.y() + (1 - rotVec.y()*rotVec.y())*c;
			rotMatrix[6] = rotVec.x()*rotVec.z() * (1 - c) - rotVec.x() * s;
			rotMatrix[8] = rotVec.x()*rotVec.z() * (1 - c) - rotVec.y() * s;
			rotMatrix[9] = rotVec.y()*rotVec.z() * (1 - c) + rotVec.x() * s;
			rotMatrix[10] = rotVec.z()*rotVec.z() + (1 - rotVec.z()*rotVec.z())*c;

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
			const auto halfFov = 0.5f * fovy;
			const auto sinfov = sinf(halfFov);
			const auto cosfov = cosf(halfFov);

			const auto height = cosfov / sinfov;
			const auto width = height / aspect;

			const auto dist = zf / (zn - zf);
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
			
			mat[1] = m[4];
			mat[2] = m[8];
			mat[3] = m[12]; 
			mat[4] = m[1];
			mat[6] = m[9];
			mat[7] = m[13];
			mat[8] = m[2];
			mat[9] = m[6];
			mat[11] = m[14];
			mat[12] = m[3];
			mat[13] = m[7];
			mat[14] = m[11];
			
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
	typedef Math::mat4 mat4;
}



