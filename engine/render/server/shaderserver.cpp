#include "config.h"
#include "shaderserver.h"
#include "render/resources/shaderobject.h"
#include <sstream>
#include <fstream>
#include <filesystem>
#include "render/render/shadervariables.h"

namespace Render
{
namespace fs = std::experimental::filesystem;
ShaderServer::ShaderServer() : stopListening(false)
{
	this->fileChangeListener = std::thread(&ShaderServer::ListenToFileChanges, this);
}

ShaderServer::~ShaderServer()
{
	CleanUp();
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

GLuint ShaderServer::LoadVertexShader(Util::String file, bool reload)
{
	if (!this->HasShaderProgramLoaded(file) || reload)
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
		const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
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

		ShaderInfo shaderInfo;
		shaderInfo.program = vertexShader;
		shaderInfo.type = GL_VERTEX_SHADER;
		shaderInfo.filename = file;
		shaderInfo.lastWriteTime = fs::last_write_time(fs::path(file.AsCharPtr())).time_since_epoch().count();

		//Insert to our shader list
		if (reload)
		{
			auto oldProgram = this->shaders[file].program;
			for(auto& shaderObj : shaderObjects.ValuesAsArray())
			{
				if (shaderObj->ContainsShader(oldProgram))
				{
					shaderObj->ReloadShaders(oldProgram, shaderInfo.program);
				}
				
			}
			this->shaders[file] = shaderInfo;
		}
		else
			this->shaders.Add(file, shaderInfo);

		return vertexShader;
	}
	
	return this->shaders[file].program;
}

GLuint ShaderServer::LoadFragmentShader(Util::String file, bool reload)
{
	if (!this->HasShaderProgramLoaded(file) || reload)
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
		const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
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

		ShaderInfo shaderInfo;
		shaderInfo.program = fragmentShader;
		shaderInfo.type = GL_FRAGMENT_SHADER;
		shaderInfo.filename = file;
		shaderInfo.lastWriteTime = fs::last_write_time(fs::path(file.AsCharPtr())).time_since_epoch().count();

		//Insert to our shader list
		if (reload)
		{
			auto oldProgram = this->shaders[file].program;
			for (auto& shaderObj : this->shaderObjects.ValuesAsArray())
			{
				shaderObj->ReloadShaders(oldProgram, shaderInfo.program);
			}
			this->shaders[file] = shaderInfo;
		}
		else
			this->shaders.Add(file, shaderInfo);

		return fragmentShader;
	}

	return this->shaders[file].program;

}

GLuint ShaderServer::LoadComputeShader(Util::String file, bool reload)
{
	if (!this->HasShaderProgramLoaded(file) || reload)
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
		const GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
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

		ShaderInfo shaderInfo;
		shaderInfo.program = computeShader;
		shaderInfo.type = GL_COMPUTE_SHADER;
		shaderInfo.filename = file;
		shaderInfo.lastWriteTime = fs::last_write_time(fs::path(file.AsCharPtr())).time_since_epoch().count();

		//Insert to our shader list
		if (reload)
		{
			const auto oldProgram = this->shaders[file].program;
			for (auto& shaderObj : shaderObjects.ValuesAsArray())
			{
				if (shaderObj->ContainsShader(oldProgram))
				{
					shaderObj->ReloadShaders(oldProgram, shaderInfo.program);
				}

			}
			this->shaders[file] = shaderInfo;
		}
		else
			this->shaders.Add(file, shaderInfo);

		return computeShader;
	}

	return this->shaders[file].program;
}

void ShaderServer::AddShaderObject(const char* name, Ptr<ShaderObject> shaderObj)
{
	this->shaderObjects.Add(name, shaderObj);
}

void ShaderServer::ListenToFileChanges()
{
	using namespace std::chrono_literals;

	while (!this->stopListening)
	{
		Util::Array<Util::String> keys = this->shaders.KeysAsArray();
		for (auto& filename : keys)
		{
			auto time = fs::last_write_time(fs::path(filename.AsCharPtr())).time_since_epoch().count();
			auto lastTime = this->shaders[filename].lastWriteTime;

			if(lastTime != 0 && lastTime < time)
			{
				this->dirtyPrograms.Append(&this->shaders[filename]);
			}
		}
		std::this_thread::sleep_for(1s);
	}
}

void ShaderServer::ReloadShader(ShaderInfo* shader)
{
	switch (shader->type)
	{
	case GL_VERTEX_SHADER: LoadVertexShader(shader->filename, true); break;
	case GL_FRAGMENT_SHADER: LoadFragmentShader(shader->filename, true); break;
	case GL_COMPUTE_SHADER: LoadComputeShader(shader->filename, true); break;
	default: printf("[SHADER RELOAD ERROR] Invalid shader type\n"); break;
	}
}

void ShaderServer::Update()
{
	std::lock_guard<std::mutex> lock_guard(this->fileChangesMutex);
	for (auto program : this->dirtyPrograms)
	{
		ReloadShader(program);
	}

	this->dirtyPrograms.Clear();
	
}

void ShaderServer::CleanUp()
{
	this->stopListening = true;
	this->fileChangeListener.join();
}

bool ShaderServer::HasShaderProgramLoaded(const Util::String& name) const
{
	return this->shaders.Contains(name);
}

bool ShaderServer::HasShaderNamed(const Util::String& name) const
{
	return this->shaderObjects.Contains(name);
}

Util::String ShaderServer::ReadFromFile(const Util::String& filename) const
{
	Util::String fileContent;
	std::ifstream fileStream(filename.AsCharPtr(), std::ios::in);

	if(!fileStream.is_open())
	{
		printf("[SHADER LOAD ERROR]: Could not load file from directory");
		return "";
	}

	std::string line = "";
	Util::String checkInclude;
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		checkInclude = line.c_str();
		if (checkInclude.ContainsCharFromSet("#"))
		{
			Util::Array<Util::String> tokens = checkInclude.Tokenize("(");
			if (tokens[0] == "#include")
			{
				Util::String filename = tokens[1].ExtractRange(1, tokens[1].Length()-3);
				line = ReadFromFile("resources/shaders/"+ filename).AsCharPtr();
			}
		}
		fileContent.Append(line.c_str() + Util::String("\n"));
	}
	fileContent.Append("\0");
	fileStream.close();

	return fileContent;
}
}
