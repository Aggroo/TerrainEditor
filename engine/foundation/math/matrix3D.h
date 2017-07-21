#pragma once
#include <iostream>
#include <math.h>
#include "vector3D.h"
#include "matrix4D.h"


namespace CGMath
{
	class matrix3D
	{
	public:
		inline matrix3D()
		{

			for (int y = 0; y < 3; y++)
			{
				for (int x = 0; x < 3; x++)
				{
					matrice[y*3 + x] = 0;
				}
			}
		}
		inline ~matrix3D()
		{

		}

		inline matrix3D operator*(matrix3D rhs)
		{
			matrix3D new_matrix;

			float value = 0;
			for (int y = 0; y < 3; y++)
			{
				for (int x = 0; x < 3; x++)
				{
					value = 0.0;
					for (int k = 0; k < 3; k++)
					{
						value += matrice[y*3 + k] * rhs[k*3 + x];
					}
					new_matrix[y*3 + x] = value;
				}
			}
			return new_matrix;
		}

		inline vector3D operator*(vector3D rhs)
		{
			vector3D new_vec;

			float value = 0;
			for (int y = 0; y < 3; y++)
			{
				value = 0.0;
				for (int k = 0; k < 3; k++)
				{
					value += matrice[y*3 + k] * rhs[k];
				}
				new_vec[y] = value;
			}
			return new_vec;
		}

		inline matrix3D operator!()
		{
			matrix3D new_matrix;
			for (int y = 0; y < 3; y++)
			{
				for (int x = 0; x < 3; x++)
				{
					new_matrix[x*3 + y] = matrice[y*3 + x];
				}
			}
			return new_matrix;
		}

		inline float &operator[](int pos)
		{
			return matrice[pos];
		}

		inline static matrix3D identity()
		{
			matrix3D identityMatrix;

			identityMatrix[0] = 1; identityMatrix[1] = 0; identityMatrix[2] = 0;
			identityMatrix[3] = 0; identityMatrix[4] = 1; identityMatrix[5] = 0;
			identityMatrix[6] = 0; identityMatrix[7] = 0; identityMatrix[8] = 1;

			return identityMatrix;
		}

		inline matrix3D invert()
		{
			float determinant, invDeterminant;
			float tmp[9];
			matrix3D new_matrix;

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

		inline static matrix3D vectorScaling(float sX, float sY, float sZ)
		{
			matrix3D scaleMatrix;

			scaleMatrix[0] = sX; scaleMatrix[4] = sY; scaleMatrix[8] = sZ;

			return scaleMatrix;
		}

		inline static matrix3D rotX(float angle)
		{
			matrix3D rotMatrix;
			float c = cos(angle);
			float s = sin(angle);

			rotMatrix[0] = 1; rotMatrix[1] = 0; rotMatrix[2] = 0;
			rotMatrix[3] = 0; rotMatrix[4] = c; rotMatrix[5] = -s;
			rotMatrix[6] = 0; rotMatrix[7] = s; rotMatrix[8] = c;

			return rotMatrix;

		}

		inline static matrix3D rotY(float angle)
		{
			matrix3D rotMatrix;
			float c = cos(angle);
			float s = sin(angle);

			rotMatrix[0] = c; rotMatrix[1] = 0; rotMatrix[2] = s;
			rotMatrix[3] = 0; rotMatrix[4] = 1; rotMatrix[5] = 0;
			rotMatrix[6] = -s; rotMatrix[7] = 0; rotMatrix[8] = c;

			return rotMatrix;
		}

		inline static matrix3D rotZ(float angle)
		{
			matrix3D rotMatrix;
			float c = cos(angle);
			float s = sin(angle);

			rotMatrix[0] = c; rotMatrix[1] = -s; rotMatrix[2] = 0;
			rotMatrix[3] = s; rotMatrix[4] = c; rotMatrix[5] = 0;
			rotMatrix[6] = 0; rotMatrix[7] = 0; rotMatrix[8] = 1;

			return rotMatrix;
		}

		inline static matrix3D vecRot(float angle, float vecX, float vecY, float vecZ)
		{
			vector3D rotVec;
			matrix3D rotMatrix;

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

		inline static matrix3D vecRot(float angle, vector3D rotVec)
		{
			matrix3D rotMatrix;

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

		inline static matrix3D Transpose(matrix3D m)
		{
			matrix3D mat;

			mat[1] = m[3];
			mat[2] = m[6];
			mat[3] = m[1];
			mat[5] = m[7];
			mat[6] = m[2];
			mat[7] = m[5];

			return mat;

		}

		inline static matrix3D fromMatrix4D(matrix4D mat4)
		{
			matrix3D newMat;
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

		inline friend std::ostream& operator<<(std::ostream& stream, const matrix3D& mat)
		{

			for (int y = 0; y < 3; y++)
			{
				for (int x = 0; x < 3; x++)
				{
					stream << mat.matrice[y*3 + x] << " ";
				}
				std::cout << std::endl;
			}
			return stream;
		}

	private:
		float matrice[9];
	};

	typedef CGMath::matrix3D mat3;
}

