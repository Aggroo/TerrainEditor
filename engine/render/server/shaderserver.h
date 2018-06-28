#pragma once
#include <GL/glew.h>
#include "core/singleton.h"
#include "core/ptr.h"
#include "foundation/util/dictionary.h"
#include "foundation/util/string.h"


namespace Render
{

class ShaderObject;

class ShaderServer
{
__DeclareSingleton(ShaderServer)
public:

	//Retrives a shader object by name
	Ptr<ShaderObject> GetShader(const char* file);

	//Load and compile a vertex shader
	GLuint LoadVertexShader(Util::String file);
	//Load and compile a fragment shader
	GLuint LoadFragmentShader(Util::String file);
	//Load and compile a compute shader
	GLuint LoadComputeShader(Util::String file);

	void AddShaderObject(const char* name, Ptr<ShaderObject> shaderObj);

	//Checks whether a shader with given name has already been loaded
	bool HasShaderProgramLoaded(const Util::String& name);
	//Checks whether a shader object with given name exists
	bool HasShaderNamed(const Util::String& name);
private:
	//Reads a shader file and returns it's content as a Util::String
	Util::String ReadFromFile(const Util::String &filename);

	//Contains all shaders. Key is shader's filename
	Util::Dictionary<Util::String, GLuint> shaders;

	//Contains all current shader objects
	//Key is filename and value is a shaderObject pointer
	Util::Dictionary<Util::String, Ptr<ShaderObject>> shaderObjects;
};
}
