#pragma once
#include <GL/glew.h>
#include "core/singleton.h"
#include "core/ptr.h"
#include "foundation/util/dictionary.h"
#include "foundation/util/string.h"

#include <mutex>

namespace Render
{

class ShaderObject;
struct RenderState;

class ShaderServer
{
__DeclareSingleton(ShaderServer)
public:
	struct ShaderInfo
	{
		GLuint program;
		GLenum type;
		Util::String filename;
		int64_t lastWriteTime;
	};
public:
	~ShaderServer();

	//Retrives a shader object by name
	Ptr<ShaderObject> GetShader(const char* name);

	//Load shader settings from json file and sets up all the shaders
	bool SetupShaders(const Util::String& file);

	//Load and compile a vertex shader
	GLuint LoadVertexShader(const Util::String& file, bool reload = false);
	//Load and compile a fragment shader
	GLuint LoadFragmentShader(const Util::String& file, bool reload = false);
	//Load and compile a compute shader
	GLuint LoadComputeShader(const Util::String& file, bool reload = false);

	Render::RenderState LoadRenderState(const Util::String& file);

	//Checks shader files for change
	void ListenToFileChanges();
	//Reloaded the updated shader
	void ReloadShader(ShaderInfo* shader);

	//Run a update each frame
	void Update();
	//Clean up the listener thread
	void CleanUp();

	//Checks whether render state with given name has already been loaded
	bool HasRenderStateLoaded(const Util::String& name) const;

	//Checks whether a shader with given name has already been loaded
	bool HasShaderProgramLoaded(const Util::String& name) const;
	//Checks whether a shader object with given name exists
	bool HasShaderNamed(const Util::String& name) const;
private:

	//Reads a shader file and returns it's content as a Util::String
	Util::String ReadFromFile(const Util::String &filename) const;

	//Contains all shaders. Key is shader's filename
	Util::Dictionary<Util::String, ShaderInfo> shaders;

	//Dirty shader programs that will get reloaded in Update()
	Util::Array<ShaderInfo*> dirtyPrograms;

	//Contains all current shader objects
	//Key is filename and value is a shaderObject pointer
	Util::Dictionary<Util::String, Ptr<ShaderObject>> shaderObjects;

	//Contains all render states
	//Key must be unique to each state, and in this case, the key is the state's filename
	Util::Dictionary<Util::String, Render::RenderState> renderStates;

	std::mutex fileChangesMutex;
	
	//Stops file listen thread
	bool stopListening;
	std::thread fileChangeListener;
};
}
