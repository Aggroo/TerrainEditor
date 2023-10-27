#pragma once
#include <math.h>
#include "vec3.h"
#include "mat4.h"


namespace Math
{
	class mat3
	{
	public:
		inline mat3()
		{
			matrice[0] = 1; matrice[1] = 0; matrice[2] = 0;
			matrice[3] = 0; matrice[4] = 1; matrice[5] = 0;
			matrice[6] = 0; matrice[7] = 0; matrice[8] = 1;
		}
		inline ~mat3()
		{

		}

		inline mat3 operator*(const mat3& rhs)
		{
			mat3 new_matrix;

			new_matrix[0] = matrice[0]*rhs[0]+matrice[1]*rhs[3]+matrice[2]*rhs[6];
			new_matrix[1] = matrice[0]*rhs[1]+matrice[1]*rhs[4]+matrice[2]*rhs[7];
			new_matrix[2] = matrice[0]*rhs[2]+matrice[1]*rhs[5]+matrice[2]*rhs[8];
			
			new_matrix[3] = matrice[3]*rhs[0]+matrice[4]*rhs[3]+matrice[5]*rhs[6];
			new_matrix[4] = matrice[3]*rhs[1]+matrice[4]*rhs[4]+matrice[5]*rhs[7];
			new_matrix[5] = matrice[3]*rhs[2]+matrice[4]*rhs[5]+matrice[5]*rhs[8];
			
			new_matrix[6] = matrice[6]*rhs[0]+matrice[7]*rhs[3]+matrice[8]*rhs[6];
			new_matrix[7] = matrice[6]*rhs[1]+matrice[7]*rhs[4]+matrice[8]*rhs[7];
			new_matrix[8] = matrice[6]*rhs[2]+matrice[7]*rhs[5]+matrice[8]*rhs[8];
			return new_matrix;
		}

		inline vec3 operator*(const vec3& rhs)
		{
			vec3 new_vec;

			new_vec[0] = matrice[0]*rhs[0]+matrice[1]*rhs[1]+matrice[2]*rhs[2];
			new_vec[1] = matrice[3]*rhs[0]+matrice[4]*rhs[1]+matrice[5]*rhs[2];
			new_vec[2] = matrice[6]*rhs[0]+matrice[7]*rhs[1]+matrice[8]*rhs[2];
			
			return new_vec;
		}

		inline mat3 operator!()
		{
			mat3 new_matrix;
			new_matrix[0] = matrice[0];
			new_matrix[1] = matrice[3];
			new_matrix[2] = matrice[6];
			
			new_matrix[3] = matrice[1];
			new_matrix[4] = matrice[4];
			new_matrix[5] = matrice[7];
			
			new_matrix[6] = matrice[2];
			new_matrix[7] = matrice[5];
			new_matrix[8] = matrice[8];
			
			return new_matrix;
		}

		inline float& operator[](const int pos)
		{
			return matrice[pos];
		}

		inline const float& operator[](const int pos) const
		{
			return matrice[pos];
		}

		inline static mat3 identity()
		{
			mat3 identityMatrix;

			identityMatrix[0] = 1; identityMatrix[1] = 0; identityMatrix[2] = 0;
			identityMatrix[3] = 0; identityMatrix[4] = 1; identityMatrix[5] = 0;
			identityMatrix[6] = 0; identityMatrix[7] = 0; identityMatrix[8] = 1;

			return identityMatrix;
		}

		inline mat3 invert()
		{
			float determinant, invDeterminant;
			float tmp[9];
			mat3 new_matrix;

			tmp[0] = matrice[4] * matrice[8] - matrice[5] * matrice[7];
			tmp[1] = matrice[2] * matrice[7] - matrice[1] * matrice[8];
			tmp[2] = matrice[1] * matrice[5] - matrice[2] * matrice[4];
			tmp[3] = matrice[5] * matrice[6] - matrice[3] * matrice[8];
			tmp[4] = matrice[0] * matrice[8] - matrice[2] * matrice[6];
			tmp[5] = matrice[2] * matrice[3] - matrice[0] * matrice[5];
			tmp[6] = matrice[3] * matrice[7] - matrice[4] * matrice[6];
			tmp[7] = matrice[1] * matrice[6] - matrice[0] * matrice[7];
			tmp[8] = matrice[0] * matrice[4] - matrice[1] * matrice[3];

			// check determinant if it is 0
			determinant = matrice[0] * tmp[0] + matrice[1] * tmp[3] + matrice[2] * tmp[6];
			if (determinant == 0)
			{
				return identity(); // cannot inverse, make it identity matrix
			}

			// divide by the determinant
			invDeterminant = 1.0f / determinant;
			
			for(int i = 0; i < 9; i++)
			{
				new_matrix[i] = invDeterminant * tmp[i];
			}

			return new_matrix;
		}

		inline static mat3 vectorScaling(const float sX, const float sY, const float sZ)
		{
			mat3 scaleMatrix;

			scaleMatrix[0] = sX; scaleMatrix[4] = sY; scaleMatrix[8] = sZ;

			return scaleMatrix;
		}

		inline static mat3 rotX(const float angle)
		{
			mat3 rotMatrix;
			float c = cos(angle);
			float s = sin(angle);

			rotMatrix[0] = 1; rotMatrix[1] = 0; rotMatrix[2] = 0;
			rotMatrix[3] = 0; rotMatrix[4] = c; rotMatrix[5] = -s;
			rotMatrix[6] = 0; rotMatrix[7] = s; rotMatrix[8] = c;

			return rotMatrix;

		}

		inline static mat3 rotY(const float angle)
		{
			mat3 rotMatrix;
			float c = cos(angle);
			float s = sin(angle);

			rotMatrix[0] = c; rotMatrix[1] = 0; rotMatrix[2] = s;
			rotMatrix[3] = 0; rotMatrix[4] = 1; rotMatrix[5] = 0;
			rotMatrix[6] = -s; rotMatrix[7] = 0; rotMatrix[8] = c;

			return rotMatrix;
		}

		inline static mat3 rotZ(const float angle)
		{
			mat3 rotMatrix;
			float c = cos(angle);
			float s = sin(angle);

			rotMatrix[0] = c; rotMatrix[1] = -s; rotMatrix[2] = 0;
			rotMatrix[3] = s; rotMatrix[4] = c; rotMatrix[5] = 0;
			rotMatrix[6] = 0; rotMatrix[7] = 0; rotMatrix[8] = 1;

			return rotMatrix;
		}

		inline static mat3 vecRot(const float angle, const float vecX, const float vecY, const float vecZ)
		{
			vec3 rotVec;
			mat3 rotMatrix;

			rotVec.x() = vecX;
			rotVec.y() = vecY;
			rotVec.z() = vecZ;

			float c = cos(angle);
			float s = sin(angle);

			rotMatrix[0] = rotVec.x()*rotVec.x() + (1 - rotVec.x()*rotVec.x())*c;
			rotMatrix[1] = rotVec.x()*rotVec.y() * (1 - c) - rotVec.x() * s;
			rotMatrix[2] = rotVec.x()*rotVec.z() * (1 - c) + rotVec.y() * s;
			rotMatrix[3] = rotVec.x()*rotVec.y() * (1 - c) + rotVec.z() * s;
			rotMatrix[4] = rotVec.y()*rotVec.y() + (1 - rotVec.y()*rotVec.y())*c;
			rotMatrix[5] = rotVec.x()*rotVec.z() * (1 - c) - rotVec.x() * s;
			rotMatrix[6] = rotVec.x()*rotVec.z() * (1 - c) - rotVec.y() * s;
			rotMatrix[7] = rotVec.y()*rotVec.z() * (1 - c) + rotVec.x() * s;
			rotMatrix[8] = rotVec.z()*rotVec.z() + (1 - rotVec.z()*rotVec.z())*c;

			return rotMatrix;
		}

		inline static mat3 vecRot(const float angle, const vec3& rotVec)
		{
			mat3 rotMatrix;

			float c = cos(angle);
			float s = sin(angle);

			rotMatrix[0] = rotVec.x()*rotVec.x() + (1 - rotVec.x()*rotVec.x())*c;
			rotMatrix[1] = rotVec.x()*rotVec.y() * (1 - c) - rotVec.x() * s;
			rotMatrix[2] = rotVec.x()*rotVec.z() * (1 - c) + rotVec.y() * s;
			rotMatrix[3] = rotVec.x()*rotVec.y() * (1 - c) + rotVec.z() * s;
			rotMatrix[4] = rotVec.y()*rotVec.y() + (1 - rotVec.y()*rotVec.y())*c;
			rotMatrix[5] = rotVec.x()*rotVec.z() * (1 - c) - rotVec.x() * s;
			rotMatrix[6] = rotVec.x()*rotVec.z() * (1 - c) - rotVec.y() * s;
			rotMatrix[7] = rotVec.y()*rotVec.z() * (1 - c) + rotVec.x() * s;
			rotMatrix[8] = rotVec.z()*rotVec.z() + (1 - rotVec.z()*rotVec.z())*c;

			return rotMatrix;
		}

		inline static mat3 Transpose(const mat3& m)
		{
			mat3 mat;

			mat[1] = m[3];
			mat[2] = m[6];
			mat[3] = m[1];
			mat[5] = m[7];
			mat[6] = m[2];
			mat[7] = m[5];

			return mat;

		}

		inline static mat3 fromMatrix4D(const mat4& mat4)
		{
			mat3 newMat;
			newMat[0] = mat4[0]; 
			newMat[1] = mat4[1]; 
			newMat[2] = mat4[2];
			newMat[3] = mat4[4]; 
			newMat[4] = mat4[5]; 
			newMat[5] = mat4[6];
			newMat[6] = mat4[8]; 
			newMat[7] = mat4[9]; 
			newMat[8] = mat4[10];

			return newMat;
		}

		inline float* getPointer()
		{

			return matrice;
		}

	private:
		float matrice[9];
	};

	typedef Math::mat3 mat3;
}

