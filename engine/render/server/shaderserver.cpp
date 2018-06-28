#include "config.h"
#include "shaderserver.h"
#include "render/resources/shaderobject.h"
#include <sstream>
#include <fstream>
#include "render/render/shadervariables.h"

namespace Render
{
ShaderServer::ShaderServer()
{
	
}

Ptr<ShaderObject> ShaderServer::GetShader(const char* file)
{
	if (this->HasShaderNamed(file))
	{
		return this->shaderObjects[file];
	}
	else
	{
		printf("[SHADER LOAD ERROR]: Could not retrieve shader!");
		_assert(false);
		return nullptr;
	}
}

GLuint ShaderServer::LoadVertexShader(Util::String file)
{
	if (!this->HasShaderProgramLoaded(file))
	{
		if (!file.CheckFileExtension("vert"))
		{
			printf("[SHADER LOAD ERROR]: File is not a .vert file");
			return false;
		}

		Util::String fileContent = ReadFromFile(file);
		if (fileContent.IsEmpty())
		{
			printf("[SHADER LOAD ERROR]: %s is empty", file.AsCharPtr());
			return false;
		}

		fileContent = vertexHeader + fileContent;
		fileContent = shaderHeader + fileContent;

		const char* vs = fileContent.AsCharPtr();

		// setup vertex shader
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		GLint length = (GLint)fileContent.Length();
		glShaderSource(vertexShader, 1, &vs, &length);
		glCompileShader(vertexShader);

		// get error log
		GLint shaderLogSize;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			char* buf = new char[shaderLogSize];
			glGetShaderInfoLog(vertexShader, shaderLogSize, NULL, buf);
			printf("[VERTEX SHADER COMPILE ERROR]: %s", buf);
			delete[] buf;
		}

		//Insert to our shader list
		this->shaders.Add(file, vertexShader);

		return vertexShader;
	}

	return this->shaders[file];
}

GLuint ShaderServer::LoadFragmentShader(Util::String file)
{
	if (!this->HasShaderProgramLoaded(file))
	{
		if (!file.CheckFileExtension("frag"))
		{
			printf("[SHADER LOAD ERROR]: File is not a .frag file");
			return false;
		}

		Util::String fileContent = ReadFromFile(file);
		if (fileContent.IsEmpty())
		{
			printf("[SHADER LOAD ERROR]: %s is empty", file.AsCharPtr());
			return false;
		}

		fileContent = fragmentHeader + fileContent;
		fileContent = shaderHeader + fileContent;

		const char* fs = fileContent.AsCharPtr();

		// setup vertex shader
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		GLint length = (GLint)fileContent.Length();
		glShaderSource(fragmentShader, 1, &fs, &length);
		glCompileShader(fragmentShader);

		// get error log
		GLint shaderLogSize;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			char* buf = new char[shaderLogSize];
			glGetShaderInfoLog(fragmentShader, shaderLogSize, NULL, buf);
			printf("[FRAGMENT SHADER COMPILE ERROR]: %s", buf);
			delete[] buf;
		}

		//Insert to our shader list
		this->shaders.Add(file, fragmentShader);

		return fragmentShader;
	}

	return this->shaders[file];

}

GLuint ShaderServer::LoadComputeShader(Util::String file)
{
	if (!this->HasShaderProgramLoaded(file))
	{
		if (!file.CheckFileExtension("comp"))
		{
			printf("[SHADER LOAD ERROR]: File is not a .comp file");
			return false;
		}

		Util::String fileContent = ReadFromFile(file);
		if (fileContent.IsEmpty())
		{
			printf("[SHADER LOAD ERROR]: %s is empty", file.AsCharPtr());
			return false;
		}

		fileContent = shaderHeader + fileContent;

		const char* cs = fileContent.AsCharPtr();

		// setup vertex shader
		GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
		GLint length = (GLint)fileContent.Length();
		glShaderSource(computeShader, 1, &cs, &length);
		glCompileShader(computeShader);

		// get error log
		GLint shaderLogSize;
		glGetShaderiv(computeShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			char* buf = new char[shaderLogSize];
			glGetShaderInfoLog(computeShader, shaderLogSize, NULL, buf);
			printf("[COMPUTE SHADER COMPILE ERROR]: %s", buf);
			delete[] buf;
		}

		//Insert to our shader list
		this->shaders.Add(file, computeShader);

		return computeShader;
	}

	return this->shaders[file];
}

void ShaderServer::AddShaderObject(const char* name, Ptr<ShaderObject> shaderObj)
{
	this->shaderObjects.Add(name, shaderObj);
}

bool ShaderServer::HasShaderProgramLoaded(const Util::String& name)
{
	return this->shaders.Contains(name);
}

bool ShaderServer::HasShaderNamed(const Util::String& name)
{
	return this->shaderObjects.Contains(name);
}

Util::String ShaderServer::ReadFromFile(const Util::String& filename)
{
	Util::String fileContent;
	std::ifstream fileStream(filename.AsCharPtr(), std::ios::in);

	if(!fileStream.is_open())
	{
		printf("[SHADER LOAD ERROR]: Could not load file from directory");
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		if (line == "#include(\"lights.frag\")")
		{
			line = ReadFromFile("resources/shaders/lights.frag").AsCharPtr();
		}
		fileContent.Append(line.c_str() + Util::String("\n"));
	}
	fileContent.Append("\0");
	fileStream.close();

	return fileContent;
}
}
