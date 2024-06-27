#pragma once
#include "GL/glew.h"
#include "foundation/math/vec4.h"
#include "foundation/math/mat3.h"
#include "foundation/math/mat4.h"
#include "core/refcounted.h"
#include "foundation/util/Variant.h"
#include "render/render/shadervariables.h"

namespace Render
{

class ShaderObject : public Core::RefCounted
{
	__DeclareClass(ShaderObject)

public:
	ShaderObject();
	~ShaderObject();

	void ReloadShaders(const GLuint& oldProgram, const GLuint& newProgram);

	//Attach shader to shader program
	void AddShader(const Util::String& shaderName, const GLuint& shaderProgram);
	void AddShader(const GLuint& shaderProgram);
	//Link the shader program 
	void LinkShaders();

	void BindProgram();
	GLuint GetProgram() const { return program; }

	void EnableRenderState();

	bool ContainsShader(const GLuint& shader);

	void setupMatrix4fv(const GLchar* name, Math::mat4 mat);
    void setupMatrix4fv(const GLchar* name, Util::Array<Math::mat4> mat, GLint count);
	void setupMatrix4fvt(const GLchar* name, Math::mat4 mat);
	void setupMatrix3fv(const GLchar* name, Math::mat3 mat);
	void setupVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	void setupVector4f(const GLchar* name, Math::vec4 vec);
	void setupVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z);
	void setupVector3f(const GLchar* name, Math::vec3 vec);
	void setupUniformFloat(const GLchar* name, GLfloat val);
	void setupUniformInt(const GLchar* name, GLint val);

private:
	friend class ShaderServer;

	void SetRenderState(const RenderState& state);

	void ReloadUniforms(const char* name, Util::Variant variance);
	bool CheckUniformMap(const Util::Array<Util::Pair<Util::String, Util::Variant>>& map, const GLchar* name);

	Util::Array<Util::Pair<Util::String, Util::Variant>> uMap;
	//Util::Dictionary<Util::String, Util::Variant> uMap;

	GLuint program;

	Util::String shaderName;

	bool reloading;

	//Array of shaders linked to program
	Util::Array<GLuint> shaders;

	Render::RenderState renderState;
};
}
