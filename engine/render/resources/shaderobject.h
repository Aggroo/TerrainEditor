#pragma once
#include "GL/glew.h"
#include "foundation/math/vec4.h"
#include "foundation/math/mat3.h"
#include "foundation/math/mat4.h"
#include "core/refcounted.h"
#include "foundation/util/Variant.h"

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
	void AddShader(const GLuint& shaderProgram);
	//Link the shader program 
	void LinkShaders();

	void BindProgram();
	GLuint GetProgram() const { return program; }

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

	void ReloadUniforms(const char* name, Util::Variant variance);

	Util::Dictionary<Util::String, Util::Variant> uMap;

	GLuint program;

	//Array of shaders linked to program
	Util::Array<GLuint> shaders;
};

inline void ShaderObject::ReloadUniforms(const char* name, Util::Variant variant)
{
	switch(variant.GetType())
	{
	case Util::Variant::Type::Void: break;
	case Util::Variant::Type::Int: setupUniformInt(name, variant.GetInt()); break;
	case Util::Variant::Type::UInt: break;
	case Util::Variant::Type::Float: setupUniformFloat(name, variant.GetFloat()); break;
	case Util::Variant::Type::Bool: break;
	case Util::Variant::Type::vec2: break;
	//case Util::Variant::Type::vec3: setupVector3f(name, variance); break;
	case Util::Variant::Type::vec4: setupVector4f(name, variant.Getvec4()); break;
	case Util::Variant::Type::String: break;
	case Util::Variant::Type::mat4: setupMatrix4fv(name, variant.Getmat4()); break;
	case Util::Variant::Type::VoidPtr:break;
	case Util::Variant::Type::IntArray: break;
	case Util::Variant::Type::FloatArray: break;
	case Util::Variant::Type::BoolArray: break;
	case Util::Variant::Type::vec2Arrayt: break;
	//case Util::Variant::Type::vec3Arrayt: break;
	case Util::Variant::Type::vec4Arrayt: break;
	case Util::Variant::Type::StringArray: break;
	case Util::Variant::Type::mat4Arrayt: break;
	default: printf("[UNIFORM VARIANT ERROR] Incorrect variant type"); break;
	}
}
}
