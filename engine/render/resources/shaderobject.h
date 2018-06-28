#pragma once
#include "GL/glew.h"
#include "foundation/math/vec4.h"
#include "foundation/math/mat3.h"
#include "foundation/math/mat4.h"
#include "core/refcounted.h"

namespace Render
{

class ShaderObject : public Core::RefCounted
{
	__DeclareClass(ShaderObject)

public:
	ShaderObject();
	~ShaderObject();

	//void ReloadShaders();

	//Attach shader to shader program
	void AddShader(const GLuint& shaderProgram);
	//Link the shader program 
	void LinkShaders();

	void BindProgram();

	void setupMatrix4fv(const GLchar* name, Math::mat4 mat);
    void setupMatrix4fv(const GLchar* name, Util::Array<Math::mat4> mat, GLint count);
	void setupMatrix4fvt(const GLchar* name, Math::mat4 mat);
	void setupMatrix3fv(const GLchar* name, Math::mat3 mat);
	void setupVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	void setupVector4f(const GLchar* name, Math::vec4 vec);
	void setupVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z);
	void setupVector3f(const GLchar* name, Math::vec3 vec);
	void setupUniformFloat(const GLchar* name, GLfloat shine);
	void setupUniformInt(const GLchar* name, GLint shine);

private:
	friend class ShaderServer;

	//Util::Dictionary<Util::String, GLuint> uMap;

	GLuint program;

	//Array of shaders linked to program
	Util::Array<GLuint> shaders;
};

}
