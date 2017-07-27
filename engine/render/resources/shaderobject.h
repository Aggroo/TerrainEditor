#pragma once
#include "GL/glew.h"
#include "config.h"
#include "foundation/math/vector4D.h"
#include "foundation/math/matrix3D.h"
#include "foundation/math/matrix4D.h"

#include <vector>
#include <map>

namespace CGMath
{
	typedef std::map<std::string, GLuint> uniformMap;
	class ShaderObject
	{
	public:
		ShaderObject();
		~ShaderObject();

		void setupShaders(const char* vertexFile, const char* fragmentFile);
		void ReloadShaders();
		void useProgram();
		void setupMatrix4fv(const GLchar* name, matrix4D mat);
        void setupMatrix4fv(const GLchar* name, std::vector<matrix4D> mat, GLint count);
		void setupMatrix4fvt(const GLchar* name, matrix4D mat);
		void setupMatrix3fv(const GLchar* name, matrix3D mat);
		void setupVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
		void setupVector4f(const GLchar* name, vector4D vec);
		void setupVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z);
		void setupVector3f(const GLchar* name, vector3D vec);
		void setupUniformFloat(const GLchar* name, GLfloat shine);
		void setupUniformInt(const GLchar* name, GLint shine);

	private:

		uniformMap uMap;
		GLuint vertexShader;
		GLuint fragmentShader;
		GLuint program;

		std::string vertexFileName;
		std::string fragmentFileName;
	};

}