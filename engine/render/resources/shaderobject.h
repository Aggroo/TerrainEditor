#pragma once
#include "GL/glew.h"
#include "config.h"
#include "foundation/math/vec4.h"
#include "foundation/math/mat3.h"
#include "foundation/math/mat4.h"
#include <vector>
#include <map>
#include "core/refcounted.h"

namespace Math
{
	typedef std::map<std::string, GLuint> uniformMap;
	class ShaderObject : public Core::RefCounted
	{
		__DeclareClass(ShaderObject)
	public:
		ShaderObject();
		~ShaderObject();

		void setupShaders(const char* vertexFile, const char* fragmentFile);
		std::string ReadInclude(const char* file);
		void ReloadShaders();
		void useProgram();
		void setupMatrix4fv(const GLchar* name, mat4 mat);
        void setupMatrix4fv(const GLchar* name, std::vector<mat4> mat, GLint count);
		void setupMatrix4fvt(const GLchar* name, mat4 mat);
		void setupMatrix3fv(const GLchar* name, mat3 mat);
		void setupVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
		void setupVector4f(const GLchar* name, vec4 vec);
		void setupVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z);
		void setupVector3f(const GLchar* name, vec3 vec);
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
