#include "config.h"
#include "shaderobject.h"
#include <iostream>
#include <string>


namespace Render
{
__ImplementClass(Render::ShaderObject, 'RESO', Core::RefCounted)

//------------------------------------------------------------------------------
/**
*/
ShaderObject::ShaderObject() : reloading(false)
{
	this->program = glCreateProgram();
}

//------------------------------------------------------------------------------
/**
*/
ShaderObject::~ShaderObject()
{
	glDeleteProgram(this->program);
}

//------------------------------------------------------------------------------
/**
*/
void ShaderObject::ReloadShaders(const GLuint& oldProgram, const GLuint& newProgram)
{
	if(ContainsShader(oldProgram))
	{
		this->shaders.Erase(this->shaders.Find(oldProgram));
		glDeleteShader(oldProgram);
		this->shaders.Append(newProgram);
		LinkShaders();
		reloading = true;
		for(int i = 0; i < this->uMap.Size(); i++)
		{
			ReloadUniforms(this->uMap[i].Key().AsCharPtr(), this->uMap[i].Value());
		}
		reloading = false;
	}	
}

//------------------------------------------------------------------------------
/**
*/
void ShaderObject::AddShader(const GLuint& shaderProgram)
{
	this->shaders.Append(shaderProgram);
	glAttachShader(this->program, shaderProgram);
}

//------------------------------------------------------------------------------
/**
*/
void ShaderObject::LinkShaders()
{
	this->program = glCreateProgram();
	for (auto &shader : shaders)
		glAttachShader(this->program, shader);

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

//------------------------------------------------------------------------------
/**
*/
void ShaderObject::BindProgram()
{
	glUseProgram(this->program);
}

//------------------------------------------------------------------------------
/**
*/
void ShaderObject::EnableRenderState()
{
	if (this->renderState.cullface == GL_TRUE)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(this->renderState.cullmode);
		glFrontFace(this->renderState.frontface);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}

	if (this->renderState.blend == GL_TRUE)
	{
		glEnable(GL_BLEND);
		glBlendFunc(this->renderState.blendsourcefunc, this->renderState.blenddestinationfunc);
	}
	else
	{
		glDisable(GL_BLEND);
	}

	if (this->renderState.alphatest == GL_TRUE)
	{
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(this->renderState.alphafunc, this->renderState.alphaclamp);
	}
	else
	{
		glDisable(GL_ALPHA_TEST);
	}

	if (this->renderState.depthtest == GL_TRUE)
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(this->renderState.depthfunc);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}
}

//------------------------------------------------------------------------------
/**
*/
bool ShaderObject::ContainsShader(const GLuint& shader)
{
	for(const auto& s : this->shaders)
	{
		if (s == shader)
			return true;
	}
	return false;
}

//------------------------------------------------------------------------------
/**
*/
void ShaderObject::setupMatrix4fv(const GLchar* name, Math::mat4 mat)
{
	GLuint shaderMatrix = glGetUniformLocation(this->program, name);
	this->BindProgram();
	glUniformMatrix4fv(shaderMatrix, 1, GL_FALSE, (!mat).getPointer());

	if (!reloading)
	{
		if(CheckUniformMap(this->uMap, name))
		{
			uMap.Append(Util::Pair<Util::String, Util::Variant>(name, Util::Variant(mat)));
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void ShaderObject::setupMatrix4fvt(const GLchar* name, Math::mat4 mat)
{
	GLuint shaderMatrix = glGetUniformLocation(this->program, name);
	this->BindProgram();
	glUniformMatrix4fv(shaderMatrix, 1, GL_TRUE, (!mat).getPointer());

	if (!reloading)
	{
		if (CheckUniformMap(this->uMap, name))
		{
			this->uMap.Append(Util::Pair<Util::String, Util::Variant>(name, Util::Variant(mat)));
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void ShaderObject::setupMatrix4fv(const GLchar* name, Util::Array<Math::mat4> mat, GLint count)
{
	GLuint shaderMatrix = glGetUniformLocation(this->program, name);
	this->BindProgram();
    glUniformMatrix4fv(shaderMatrix, count, GL_TRUE, (float*)&mat[0]);

	if (!reloading)
	{
		if (CheckUniformMap(this->uMap, name))
		{
			this->uMap.Append(Util::Pair<Util::String, Util::Variant>(name, Util::Variant(mat)));
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void ShaderObject::setupMatrix3fv(const GLchar* name, Math::mat3 mat)
{
	GLuint shaderMatrix = glGetUniformLocation(this->program, name);
	this->BindProgram();
	glUniformMatrix3fv(shaderMatrix, 1, GL_FALSE, (!mat).getPointer());
}

//------------------------------------------------------------------------------
/**
*/
void ShaderObject::setupVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
	GLuint shaderMatrix = glGetUniformLocation(this->program, name);
	this->BindProgram();
	glUniform4f(shaderMatrix, x, y, z, w);

	if (!reloading)
	{
		if (CheckUniformMap(this->uMap, name))
		{
			this->uMap.Append(Util::Pair<Util::String, Util::Variant>(name, Util::Variant(Math::vec4(x, y, z, w))));
		}
	}

}

//------------------------------------------------------------------------------
/**
*/
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

	if (!reloading)
	{
		if (CheckUniformMap(this->uMap, name))
		{
			this->uMap.Append(Util::Pair<Util::String, Util::Variant>(name, Util::Variant(vec)));
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void ShaderObject::setupVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z)
{
	GLuint shaderMatrix = glGetUniformLocation(this->program, name);
	this->BindProgram();
	glUniform3f(shaderMatrix, x, y, z);
	//uMap[name] = Math::vec3(x,y,z);
}

//------------------------------------------------------------------------------
/**
*/
void ShaderObject::setupVector3f(const GLchar* name, Math::vec3 vec)
{
	GLuint shaderMatrix = glGetUniformLocation(this->program, name);
	this->BindProgram();
	glUniform3f(shaderMatrix, vec[0], vec[1], vec[2]);
	//uMap[name] = vec;
}

//------------------------------------------------------------------------------
/**
*/
void ShaderObject::setupUniformFloat(const GLchar* name, GLfloat val)
{
	GLuint shaderMatrix = glGetUniformLocation(this->program, name);
	this->BindProgram();
	glUniform1f(shaderMatrix, val);
		
	if (!reloading)
	{
		if (CheckUniformMap(this->uMap, name))
		{
			this->uMap.Append(Util::Pair<Util::String, Util::Variant>(name, Util::Variant(val)));
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void ShaderObject::setupUniformInt(const GLchar *name, GLint val)
{
	GLuint shaderMatrix = glGetUniformLocation(this->program, name);
	this->BindProgram();
	glUniform1i(shaderMatrix, val);

	if (!reloading)
	{
		if(CheckUniformMap(this->uMap, name))
		{
			this->uMap.Append(Util::Pair<Util::String, Util::Variant>(name, Util::Variant(val)));
		}
	}
		
}

//------------------------------------------------------------------------------
/**
*/
void ShaderObject::SetRenderState(const RenderState & state)
{
	this->renderState = state;
}

//------------------------------------------------------------------------------
/**
*/
void ShaderObject::ReloadUniforms(const char* name, Util::Variant variant)
{
	switch (variant.GetType())
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

//------------------------------------------------------------------------------
/**
*/
bool ShaderObject::CheckUniformMap(const Util::Array<Util::Pair<Util::String, Util::Variant>>& map, const GLchar *name)
{
	for (auto pair : map)
	{
		if (pair.Key() == Util::String(name))
		{
			return false;
		}
	}

	return true;
}
}
