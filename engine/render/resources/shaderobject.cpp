#include "config.h"
#include "shaderobject.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

namespace CGMath
{
	ShaderObject::ShaderObject()
	{

	}

	ShaderObject::~ShaderObject()
	{
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteProgram(this->program);
	}

	void ShaderObject::setupShaders(const char* vertexFile, const char* fragmentFile)
	{
		vertexFileName = vertexFile;
		fragmentFileName = fragmentFile;

		std::string vert;
		std::ifstream vertStream(vertexFile, std::ios::in);

		if (!vertStream.is_open()) {
			std::cerr << "Could not read file " << vertexFile << ". File does not exist." << std::endl;
			
		}

		std::string vertline = "";
		while (!vertStream.eof()) {
			std::getline(vertStream, vertline);
			vert.append(vertline + "\n");
		}
		vert.append("\0");
		vertStream.close();

		const char* vs = vert.c_str();

		std::string frag;
		std::ifstream fragStream(fragmentFile, std::ios::in);

		if (!fragStream.is_open()) {
			std::cerr << "Could not read file " << fragmentFile << ". File does not exist." << std::endl;
			
		}

		std::string fragline = "";
		while (!fragStream.eof()) {
			std::getline(fragStream, fragline);
			frag.append(fragline + "\n");
		}
		frag.append("\0");
		fragStream.close();

		const char* fs = frag.c_str();

		// setup vertex shader
		this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
		GLint length = std::strlen(vs);
		glShaderSource(this->vertexShader, 1, &vs, &length);
		glCompileShader(this->vertexShader);

		// get error log
		GLint shaderLogSize;
		glGetShaderiv(this->vertexShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetShaderInfoLog(this->vertexShader, shaderLogSize, NULL, buf);
			printf("[SHADER COMPILE ERROR]: %s", buf);
			delete[] buf;
		}

		// setup fragment shader
		this->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		length = std::strlen(fs);
		glShaderSource(this->fragmentShader, 1, &fs, &length);
		glCompileShader(this->fragmentShader);

		// get error log
		shaderLogSize;
		glGetShaderiv(this->fragmentShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetShaderInfoLog(this->fragmentShader, shaderLogSize, NULL, buf);
			printf("[SHADER COMPILE ERROR]: %s", buf);
			delete[] buf;
		}

		this->program = glCreateProgram();
		glAttachShader(this->program, this->vertexShader);
		glAttachShader(this->program, this->fragmentShader);
		glLinkProgram(this->program);
		glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetProgramInfoLog(this->program, shaderLogSize, NULL, buf);
			printf("[PROGRAM LINK ERROR]: %s", buf);
			delete[] buf;
		}

	}

	void ShaderObject::ReloadShaders()
	{

		std::string vert;
		std::ifstream vertStream(vertexFileName, std::ios::in);

		if (!vertStream.is_open()) {
			std::cerr << "Could not read file " << vertexFileName << ". File does not exist." << std::endl;

		}

		std::string vertline = "";
		while (!vertStream.eof()) {
			std::getline(vertStream, vertline);
			vert.append(vertline + "\n");
		}
		vert.append("\0");
		vertStream.close();

		const char* vs = vert.c_str();

		std::string frag;
		std::ifstream fragStream(fragmentFileName, std::ios::in);

		if (!fragStream.is_open()) {
			std::cerr << "Could not read file " << fragmentFileName << ". File does not exist." << std::endl;

		}

		std::string fragline = "";
		while (!fragStream.eof()) {
			std::getline(fragStream, fragline);
			frag.append(fragline + "\n");
		}
		frag.append("\0");
		fragStream.close();

		const char* fs = frag.c_str();

		GLint length = std::strlen(vs);
		glShaderSource(this->vertexShader, 1, &vs, &length);
		glCompileShader(this->vertexShader);

		// get error log
		GLint shaderLogSize;
		glGetShaderiv(this->vertexShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetShaderInfoLog(this->vertexShader, shaderLogSize, NULL, buf);
			printf("[SHADER COMPILE ERROR]: %s", buf);
			delete[] buf;
		}

		// setup fragment shader
		length = std::strlen(fs);
		glShaderSource(this->fragmentShader, 1, &fs, &length);
		glCompileShader(this->fragmentShader);

		// get error log
		glGetShaderiv(this->fragmentShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetShaderInfoLog(this->fragmentShader, shaderLogSize, NULL, buf);
			printf("[SHADER COMPILE ERROR]: %s", buf);
			delete[] buf;
		}

		glLinkProgram(this->program);
		glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetProgramInfoLog(this->program, shaderLogSize, NULL, buf);
			printf("[PROGRAM LINK ERROR]: %s", buf);
			delete[] buf;
		}
	}

	void ShaderObject::useProgram()
	{
		glUseProgram(this->program);
	}

	void ShaderObject::setupMatrix4fv(const GLchar* name, matrix4D mat)
	{
		std::string temp = name;
		if (uMap.find(temp) == uMap.end())
		{
			GLuint shaderMatrix = glGetUniformLocation(this->program, name);
			uMap[temp] = shaderMatrix;
		}

		this->useProgram();
		glUniformMatrix4fv(uMap[temp], 1, GL_FALSE, (!mat).getPointer());
	}

	void ShaderObject::setupMatrix4fvt(const GLchar* name, matrix4D mat)
	{
		std::string temp = name;
		if (uMap.find(temp) == uMap.end())
		{
			GLuint shaderMatrix = glGetUniformLocation(this->program, name);
			uMap[temp] = shaderMatrix;
		}

		this->useProgram();
		glUniformMatrix4fv(uMap[temp], 1, GL_TRUE, (!mat).getPointer());
	}
    void ShaderObject::setupMatrix4fv(const GLchar* name, std::vector<matrix4D> mat, GLint count)
    {
        std::string temp = name;
        if (uMap.find(temp) == uMap.end())
        {
            GLuint shaderMatrix = glGetUniformLocation(this->program, name);
            uMap[temp] = shaderMatrix;
        }

        this->useProgram();
        glUniformMatrix4fv(uMap[temp], count, GL_TRUE, (float*)&mat[0]);
    }

	void ShaderObject::setupMatrix3fv(const GLchar* name, matrix3D mat)
	{
		std::string temp = name;
		if (uMap.find(temp) == uMap.end())
		{
			GLuint shaderMatrix = glGetUniformLocation(this->program, name);
			uMap[temp] = shaderMatrix;
		}

		this->useProgram();
		glUniformMatrix3fv(uMap[temp], 1, GL_FALSE, (!mat).getPointer());
	}

	void ShaderObject::setupVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
	{
		std::string temp = name;
		if (uMap.find(temp) == uMap.end())
		{
			GLuint shaderMatrix = glGetUniformLocation(this->program, name);
			uMap[temp] = shaderMatrix;
		}

		this->useProgram();
		glUniform4f(uMap[temp], x, y, z, w);
	}

	void ShaderObject::setupVector4f(const GLchar* name, vector4D vec)
	{
		GLfloat x, y, z, w;

		x = vec.x();
		y = vec.y();
		z = vec.z();
		w = vec.w();
		
		std::string temp = name;
		if (uMap.find(temp) == uMap.end())
		{
			GLuint shaderMatrix = glGetUniformLocation(this->program, name);
			uMap[temp] = shaderMatrix;
		}

		this->useProgram();
		glUniform4f(uMap[temp], x, y, z, w);
		
	}


	void ShaderObject::setupVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z)
	{
		std::string temp = name;
		if (uMap.find(temp) == uMap.end())
		{
			GLuint shaderMatrix = glGetUniformLocation(this->program, name);
			uMap[temp] = shaderMatrix;
		}

		this->useProgram();
		glUniform3f(uMap[temp], x, y, z);
	}

	void ShaderObject::setupVector3f(const GLchar* name, vector3D vec)
	{
		std::string temp = name;
		if (uMap.find(temp) == uMap.end())
		{
			GLuint shaderMatrix = glGetUniformLocation(this->program, name);
			uMap[temp] = shaderMatrix;
		}

		this->useProgram();
		glUniform3f(uMap[temp], vec[0], vec[1], vec[2]);
	}

	void ShaderObject::setupUniformFloat(const GLchar* name, GLfloat shine)
	{
		std::string temp = name;
		if (uMap.find(temp) == uMap.end())
		{
			GLuint shaderMatrix = glGetUniformLocation(this->program, name);
			uMap[temp] = shaderMatrix;
		}

		this->useProgram();
		glUniform1f(uMap[temp], shine);
	}

	void ShaderObject::setupUniformInt(const GLchar *name, GLint shine)
	{
		std::string temp = name;
		if (uMap.find(temp) == uMap.end())
		{
			GLuint shaderMatrix = glGetUniformLocation(this->program, name);
			uMap[temp] = shaderMatrix;
		}

		this->useProgram();
		glUniform1i(uMap[temp], shine);
	}

}