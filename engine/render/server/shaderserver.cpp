#include "config.h"
#include "shaderserver.h"
#include "render/resources/shaderobject.h"
#include <sstream>
#include <fstream>
#include <filesystem>
#include "render/render/shadervariables.h"
#include "render/resources/materialcomponents.h"

using json = nlohmann::json;

namespace Render
{
namespace fs = std::filesystem;
//------------------------------------------------------------------------------
/**
*/
ShaderServer::ShaderServer() : stopListening(false)
{
	this->fileChangeListener = std::thread(&ShaderServer::ListenToFileChanges, this);
}

//------------------------------------------------------------------------------
/**
*/
ShaderServer::~ShaderServer()
{
	CleanUp();
}

//------------------------------------------------------------------------------
/**
*/
Ptr<ShaderObject> ShaderServer::GetShader(const char* name)
{
	if (this->HasShaderNamed(name))
	{
		return this->shaderObjects[name];
	}
	else
	{
		T_CORE_ERROR("[SHADER LOAD ERROR]: Could not retrieve shader!");
		_assert(false);
		return nullptr;
	}
}

//------------------------------------------------------------------------------
/**
*/
bool ShaderServer::SetupShaders(const Util::String & file)
{
	std::ifstream i(file.AsCharPtr());

	if (!i) {
		T_CORE_ERROR("[SHADER LOAD]: Couldn't find shaders.json!");
		_assert(false);
		return false;
	}

	json j;
	i >> j;

	this->shaderObjects.BeginBulkAdd();

	for (auto& element : j.at("ComputeShaders"))
	{
		auto shader = element.get<Components::ComputeShader>();

		if (this->HasShaderNamed(shader.name.c_str()))
		{
			T_CORE_WARN("[Duplicate shader loaded]: \" {0} \". Using previously loaded shader...", shader.name.c_str());
		}
		else
		{
			Ptr<ShaderObject> shd = ShaderObject::Create();

			GLuint comp = LoadComputeShader(shader.shader.c_str());

			shd->AddShader(comp);

			shd->LinkShaders();

			this->shaderObjects.Add(shader.name.c_str(), shd);
		}
	}

	for (auto& element : j.at("Shaders")) 
	{
		auto shader = element.get<Components::Shader>();

		if (this->HasShaderNamed(shader.name.c_str()))
		{
			T_CORE_WARN("[Duplicate shader loaded]: \" {0} \". Using previously loaded shader...", shader.name.c_str());
		}
		else
		{
			Ptr<ShaderObject> shd = ShaderObject::Create();

			GLuint vert = LoadVertexShader(shader.vertexShader.c_str());
			GLuint frag = LoadFragmentShader(shader.fragmentShader.c_str());

			shd->AddShader(vert);
			shd->AddShader(frag);

			Render::RenderState states = LoadRenderState(shader.renderState.c_str());
			shd->SetRenderState(states);

			shd->LinkShaders();

			this->shaderObjects.Add(shader.name.c_str(), shd);
		}
	}

	this->shaderObjects.EndBulkAdd();

	return true;
}

//------------------------------------------------------------------------------
/**
*/
GLuint ShaderServer::LoadVertexShader(const Util::String& file, bool reload)
{
	if (!this->HasShaderProgramLoaded(file) || reload)
	{
		if (!file.CheckFileExtension("vert"))
		{
			T_CORE_ERROR("[SHADER LOAD]: File is not a .vert file");
			return false;
		}

		Util::String fileContent = ReadFromFile(file);
		if (fileContent.IsEmpty())
		{
			T_CORE_ERROR("[SHADER LOAD]: {0} is empty", file.AsCharPtr());
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
			T_CORE_ERROR("[VERTEX SHADER COMPILE]: {0}", buf);
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

//------------------------------------------------------------------------------
/**
*/
GLuint ShaderServer::LoadFragmentShader(const Util::String& file, bool reload)
{
	if (!this->HasShaderProgramLoaded(file) || reload)
	{

		if (!file.CheckFileExtension("frag"))
		{
			T_CORE_ERROR("[SHADER LOAD]: File is not a .frag file");
			return false;
		}

		Util::String fileContent = ReadFromFile(file);
		if (fileContent.IsEmpty())
		{
			T_CORE_ERROR("[SHADER LOAD]: {0} is empty", file.AsCharPtr());
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
			T_CORE_ERROR("[FRAGMENT SHADER COMPILE]: ({0}), {1}", file.AsCharPtr(), buf);
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

//------------------------------------------------------------------------------
/**
*/
GLuint ShaderServer::LoadComputeShader(const Util::String& file, bool reload)
{
	if (!this->HasShaderProgramLoaded(file) || reload)
	{
		if (!file.CheckFileExtension("comp"))
		{
			T_CORE_ERROR("[SHADER LOAD]: File is not a .comp file");
			return false;
		}

		Util::String fileContent = ReadFromFile(file);
		if (fileContent.IsEmpty())
		{
			T_CORE_ERROR("[SHADER LOAD]: {0} is empty", file.AsCharPtr());
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
			T_CORE_ERROR("[COMPUTE SHADER COMPILE]: {0}", buf);
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

//------------------------------------------------------------------------------
/**
* TODO: Look over the file structure of the render states
*/
Render::RenderState ShaderServer::LoadRenderState(const Util::String & file)
{
	//Make sure we've not already loaded this render state
	if (!this->HasRenderStateLoaded(file))
	{
		if (!file.CheckFileExtension("state"))
		{
			T_CORE_ERROR("[SHADER LOAD]: File is not a .state file!");
			_assert(false);
			return RenderState();
		}

		RenderState state;

		std::ifstream fin(file.AsCharPtr());
		std::string line;
		std::istringstream sin;

		std::string sVal;
		float fVal;

		while (std::getline(fin, line))
		{
			sin.str(line.substr(line.find("=") + 1));
			if (line.find("CullFace ") != std::string::npos)
			{
				sin >> sVal;
				GLboolean glVal;

				if (sVal == "true") { glVal = GL_TRUE; }
				else if (sVal == "false") { glVal = GL_FALSE; }
				else { _assert2(false, "[SHADER LOAD ERROR]: [STATE LOAD]: Syntax error in CullFace value!\n"); }

				state.cullface = glVal;
			}
			else if (line.find("FrontFace ") != std::string::npos)
			{
				sin >> sVal;
				GLenum glVal;

				if (sVal == "cw") { glVal = GL_CW; }
				else if (sVal == "ccw") { glVal = GL_CCW; }
				else { _assert2(false, "[SHADER LOAD ERROR]: [STATE LOAD]: Syntax error in FrontFace value!\n"); }

				state.frontface = glVal;
			}
			else if (line.find("CullMode ") != std::string::npos)
			{
				sin >> sVal;
				GLenum glVal;

				if (sVal == "back") { glVal = GL_BACK; }
				else if (sVal == "front") { glVal = GL_FRONT; }
				else if (sVal == "front_and_back") { glVal = GL_FRONT_AND_BACK; }
				else { _assert2(false, "[SHADER LOAD ERROR]: [STATE LOAD]: Syntax error in CullMode value!\n"); }

				state.cullmode = glVal;
			}
			else if (line.find("Blend ") != std::string::npos)
			{
				sin >> sVal;
				GLboolean glVal;

				if (sVal == "true") { glVal = GL_TRUE; }
				else if (sVal == "false") { glVal = GL_FALSE; }
				else { _assert2(false, "[SHADER LOAD ERROR]: [STATE LOAD]: Syntax error in Blend value!\n"); }

				state.blend = glVal;
			}
			else if (line.find("BlendSourceFunc ") != std::string::npos)
			{
				sin >> sVal;
				GLenum glVal;

				if (sVal == "zero") { glVal = GL_ZERO; }
				else if (sVal == "one") { glVal = GL_ONE; }
				else if (sVal == "src_color") { glVal = GL_SRC_COLOR; }
				else if (sVal == "one_minus_src_color") { glVal = GL_ONE_MINUS_SRC_COLOR; }
				else if (sVal == "dst_color") { glVal = GL_DST_COLOR; }
				else if (sVal == "one_minus_dst_color") { glVal = GL_ONE_MINUS_DST_COLOR; }
				else if (sVal == "src_alpha") { glVal = GL_SRC_ALPHA; }
				else if (sVal == "one_minus_src_alpha") { glVal = GL_ONE_MINUS_SRC_ALPHA; }
				else if (sVal == "dst_alpha") { glVal = GL_DST_ALPHA; }
				else if (sVal == "one_minus_dst_alpha") { glVal = GL_ONE_MINUS_DST_ALPHA; }
				else if (sVal == "constant_color") { glVal = GL_CONSTANT_COLOR; }
				else if (sVal == "one_minus_constant_color") { glVal = GL_ONE_MINUS_CONSTANT_COLOR; }
				else if (sVal == "constant_alpha") { glVal = GL_CONSTANT_ALPHA; }
				else if (sVal == "one_minus_constant_alpha") { glVal = GL_ONE_MINUS_CONSTANT_ALPHA; }
				else if (sVal == "src_alpha_saturate") { glVal = GL_SRC_ALPHA_SATURATE; }
				else { _assert2(false, "[SHADER LOAD ERROR]: [STATE LOAD]: Syntax error in BlendSourceFunc value!\n"); }

				state.blendsourcefunc = glVal;
			}
			else if (line.find("BlendDestinationFunc ") != std::string::npos)
			{
				sin >> sVal;
				GLenum glVal;

				if (sVal == "zero") { glVal = GL_ZERO; }
				else if (sVal == "one") { glVal = GL_ONE; }
				else if (sVal == "src_color") { glVal = GL_SRC_COLOR; }
				else if (sVal == "one_minus_src_color") { glVal = GL_ONE_MINUS_SRC_COLOR; }
				else if (sVal == "dst_color") { glVal = GL_DST_COLOR; }
				else if (sVal == "one_minus_dst_color") { glVal = GL_ONE_MINUS_DST_COLOR; }
				else if (sVal == "src_alpha") { glVal = GL_SRC_ALPHA; }
				else if (sVal == "one_minus_src_alpha") { glVal = GL_ONE_MINUS_SRC_ALPHA; }
				else if (sVal == "dst_alpha") { glVal = GL_DST_ALPHA; }
				else if (sVal == "one_minus_dst_alpha") { glVal = GL_ONE_MINUS_DST_ALPHA; }
				else if (sVal == "constant_color") { glVal = GL_CONSTANT_COLOR; }
				else if (sVal == "one_minus_constant_color") { glVal = GL_ONE_MINUS_CONSTANT_COLOR; }
				else if (sVal == "constant_alpha") { glVal = GL_CONSTANT_ALPHA; }
				else if (sVal == "one_minus_constant_alpha") { glVal = GL_ONE_MINUS_CONSTANT_ALPHA; }
				else { _assert2(false, "[SHADER LOAD ERROR]: [STATE LOAD]: Syntax error in BlendSourceFunc value!\n"); }

				state.blenddestinationfunc = glVal;
			}
			else if (line.find("AlphaTest ") != std::string::npos)
			{
				sin >> sVal;
				GLboolean glVal;

				if (sVal == "true") { glVal = GL_TRUE; }
				else if (sVal == "false") { glVal = GL_FALSE; }
				else { _assert2(false, "[SHADER LOAD ERROR]: [STATE LOAD]: Syntax error in AlphaTest value!\n"); }

				state.alphatest = glVal;
			}
			else if (line.find("AlphaFunc ") != std::string::npos)
			{
				sin >> sVal;
				GLenum glVal;

				if (sVal == "never") { glVal = GL_NEVER; }
				else if (sVal == "less") { glVal = GL_LESS; }
				else if (sVal == "equal") { glVal = GL_EQUAL; }
				else if (sVal == "lequal") { glVal = GL_LEQUAL; }
				else if (sVal == "greater") { glVal = GL_GREATER; }
				else if (sVal == "notequal") { glVal = GL_NOTEQUAL; }
				else if (sVal == "gequal") { glVal = GL_GEQUAL; }
				else if (sVal == "always") { glVal = GL_ALWAYS; }
				else { _assert2(false, "[SHADER LOAD ERROR]: [STATE LOAD]: Syntax error in AlphaFunc value!\n"); }

				state.alphafunc = glVal;
			}
			else if (line.find("AlphaClamp ") != std::string::npos)
			{
				sin >> fVal;
				_assert2((fVal > 0.0f || fVal < 1.0f), "[SHADER LOAD ERROR]: [STATE LOAD]: AlphaClamp value can not be less than 0.0 or greater than 1.0!\n");
				state.alphaclamp = fVal;
			}
			else if (line.find("DepthTest ") != std::string::npos)
			{
				sin >> sVal;
				GLboolean glVal;

				if (sVal == "true") { glVal = GL_TRUE; }
				else if (sVal == "false") { glVal = GL_FALSE; }
				else { _assert2(false, "[SHADER LOAD ERROR]: [STATE LOAD]: Syntax error in DepthTest value!\n"); }

				state.depthtest = glVal;
			}
			else if (line.find("DepthFunc ") != std::string::npos)
			{
				sin >> sVal;
				GLenum glVal;

				if (sVal == "never") { glVal = GL_NEVER; }
				else if (sVal == "less") { glVal = GL_LESS; }
				else if (sVal == "equal") { glVal = GL_EQUAL; }
				else if (sVal == "lequal") { glVal = GL_LEQUAL; }
				else if (sVal == "greater") { glVal = GL_GREATER; }
				else if (sVal == "notequal") { glVal = GL_NOTEQUAL; }
				else if (sVal == "gequal") { glVal = GL_GEQUAL; }
				else if (sVal == "always") { glVal = GL_ALWAYS; }
				else { _assert2(false, "[SHADER LOAD ERROR]: [STATE LOAD]: Syntax error in DepthFunc value!\n"); }

				state.depthfunc = glVal;
			}
			else if (line.find("DepthWrite ") != std::string::npos)
			{
				sin >> sVal;
				GLboolean glVal;

				if (sVal == "true") { glVal = GL_TRUE; }
				else if (sVal == "false") { glVal = GL_FALSE; }
				else { _assert2(false, "[SHADER LOAD ERROR]: [STATE LOAD]: Syntax error in DepthWrite value!\n"); }

				state.depthwrite = glVal;
			}

			sin.clear();
		}

		this->renderStates.Add(file, state);
		return state;
	}
	else
	{
		//Return existing render state from list
		return this->renderStates[file];
	}
}

//------------------------------------------------------------------------------
/**
*/
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

//------------------------------------------------------------------------------
/**
*/
void ShaderServer::ReloadShader(ShaderInfo* shader)
{
	switch (shader->type)
	{
	case GL_VERTEX_SHADER: LoadVertexShader(shader->filename, true); break;
	case GL_FRAGMENT_SHADER: LoadFragmentShader(shader->filename, true); break;
	case GL_COMPUTE_SHADER: LoadComputeShader(shader->filename, true); break;
	default: T_CORE_ERROR("[SHADER RELOAD]: Invalid shader type"); break;
	}
}

//------------------------------------------------------------------------------
/**
*/
void ShaderServer::Update()
{
	std::lock_guard<std::mutex> lock_guard(this->fileChangesMutex);
	for (auto program : this->dirtyPrograms)
	{
		ReloadShader(program);
	}

	this->dirtyPrograms.Clear();
	
}

//------------------------------------------------------------------------------
/**
*/
void ShaderServer::CleanUp()
{
	this->stopListening = true;
	this->fileChangeListener.join();
}

//------------------------------------------------------------------------------
/**
*/
bool ShaderServer::HasRenderStateLoaded(const Util::String& name) const
{
	return this->renderStates.Contains(name);
}

//------------------------------------------------------------------------------
/**
*/
bool ShaderServer::HasShaderProgramLoaded(const Util::String& name) const
{
	return this->shaders.Contains(name);
}

//------------------------------------------------------------------------------
/**
*/
bool ShaderServer::HasShaderNamed(const Util::String& name) const
{
	return this->shaderObjects.Contains(name);
}

//------------------------------------------------------------------------------
/**
*/
Util::String ShaderServer::ReadFromFile(const Util::String& filename) const
{
	Util::String fileContent;
	std::ifstream fileStream(filename.AsCharPtr(), std::ios::in);

	if(!fileStream.is_open())
	{
		T_CORE_ERROR("[SHADER LOAD]: Could not load file from directory");
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
