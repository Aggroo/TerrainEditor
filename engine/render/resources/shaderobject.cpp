#include "config.h"
#include "shaderobject.h"
#include <iostream>
#include <string>


namespace Render
{
__ImplementClass(Render::ShaderObject, 'RESO', Core::RefCounted)

ShaderObject::ShaderObject()
{
	this->program = glCreateProgram();
}

ShaderObject::~ShaderObject()
{
	glDeleteProgram(this->program);
}

void ShaderObject::AddShader(const GLuint& shaderProgram)
{
	this->shaders.Append(shaderProgram);
	glAttachShader(this->program, shaderProgram);
}

void ShaderObject::LinkShaders()
{
	glLinkProgram(this->program);
	GLint shaderLogSize;
	glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &shaderLogSize);
	if (shaderLogSize > 0)
	{
		GLchar* buf = new GLchar[shaderLogSize];
		glGetProgramInfoLog(this->program, shaderLogSize, NULL, buf);
		printf("[PROGRAM LINK ERROR]: %s", buf);
		delete[] buf;
	}
}

void ShaderObject::BindProgram()
{
	glUseProgram(this->program);
}

void ShaderObject::setupMatrix4fv(const GLchar* name, Math::mat4 mat)
{
	GLuint shaderMatrix = glGetUniformLocation(this->program, name);
	this->BindProgram();
	glUniformMatrix4fv(shaderMatrix, 1, GL_FALSE, (!mat).getPointer());
}

void ShaderObject::setupMatrix4fvt(const GLchar* name, Math::mat4 mat)
{
	GLuint shaderMatrix = glGetUniformLocation(this->program, name);
	this->BindProgram();
	glUniformMatrix4fv(shaderMatrix, 1, GL_TRUE, (!mat).getPointer());
}
void ShaderObject::setupMatrix4fv(const GLchar* name, Util::Array<Math::mat4> mat, GLint count)
{
	GLuint shaderMatrix = glGetUniformLocation(this->program, name);
	this->BindProgram();
    glUniformMatrix4fv(shaderMatrix, count, GL_TRUE, (float*)&mat[0]);
}

void ShaderObject::setupMatrix3fv(const GLchar* name, Math::mat3 mat)
{
	GLuint shaderMatrix = glGetUniformLocation(this->program, name);
	this->BindProgram();
	glUniformMatrix3fv(shaderMatrix, 1, GL_FALSE, (!mat).getPointer());
}

void ShaderObject::setupVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
	GLuint shaderMatrix = glGetUniformLocation(this->program, name);
	this->BindProgram();
	glUniform4f(shaderMatrix, x, y, z, w);
}

void ShaderObject::setupVector4f(const GLchar* name, Math::vec4 vec)
{
	GLfloat x, y, z, w;

	x = vec.x();
	y = vec.y();
	z = vec.z();
	w = vec.w();
	
	GLuint shaderMatrix = glGetUniformLocation(this->program, name);
	this->BindProgram();
	glUniform4f(shaderMatrix, x, y, z, w);
	
}


void ShaderObject::setupVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z)
{
	GLuint shaderMatrix = glGetUniformLocation(this->program, name);
	this->BindProgram();
	glUniform3f(shaderMatrix, x, y, z);
}

void ShaderObject::setupVector3f(const GLchar* name, Math::vec3 vec)
{
	GLuint shaderMatrix = glGetUniformLocation(this->program, name);
	this->BindProgram();
	glUniform3f(shaderMatrix, vec[0], vec[1], vec[2]);
}

void ShaderObject::setupUniformFloat(const GLchar* name, GLfloat shine)
{
	GLuint shaderMatrix = glGetUniformLocation(this->program, name);
	this->BindProgram();
	glUniform1f(shaderMatrix, shine);
}

void ShaderObject::setupUniformInt(const GLchar *name, GLint shine)
{
	GLuint shaderMatrix = glGetUniformLocation(this->program, name);
	this->BindProgram();
	glUniform1i(shaderMatrix, shine);
}

}
