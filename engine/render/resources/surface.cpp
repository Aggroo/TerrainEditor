#include "surface.h"
//------------------------------------------------------------------------------
//  surface.cc
//  (C) 2020 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "surface.h"
#include "material.h"
#include "texturenode.h"
#include "foundation/util/variant.h"
#include "model.h"

namespace Render
{
__ImplementClass(Render::Surface, 'SRFC', Core::RefCounted);

Surface::Surface()
{
	this->textures = Render::TextureNode::Create();
}

Surface::~Surface()
{
}

void Surface::SetupShaderUniforms()
{
	Util::Array<Ptr<ShaderObject>> shaders = this->material->GetShaders();

	for (size_t i = 0; i < shaders.Size(); i++)
	{
		shaders[i]->BindProgram();

		for (auto param : parameters)
		{
			switch (param->var.GetType())
			{
			case Util::Variant::Type::String:
				shaders[i]->setupUniformInt(this->material->UniformNameFromString(param->name).AsCharPtr(), (GLuint)this->material->TextureIndexFromString(param->name));
				break;
			case Util::Variant::Type::Float:
				shaders[i]->setupUniformFloat(param->name.AsCharPtr(), param->var.GetFloat());
				break;
			case Util::Variant::Type::vec4:
				shaders[i]->setupVector4f(param->name.AsCharPtr(), param->var.Getvec4());
				break;

			default:
				printf("ERROR : Parameter might not be fully implemented! \n");
				assert(false);
				break;
			}
		}
	}
}

Render::MaterialParameter* Surface::GetParameterByName(const Util::String & name)
{
	return this->parametersByName[name];
}

void Surface::AddEntity(Ptr<Render::Model> entity)
{
	this->entites.Append(entity);
}

const Util::Array<Ptr<Render::Model>>& Surface::GetEntites()
{
	return this->entites;
}

void Surface::AddParameter(const Util::String & name, const Util::Variant & variable)
{
	if (variable.GetType() == Util::Variant::Type::String)
	{
		//HACK: Since we're loading a string, we're probably loading a texture
		//This should probably be done in some other way
		this->textures->AddTexture(Material::TextureIndexFromString(name.AsCharPtr()), variable.GetString().AsCharPtr());
	}
	else
	{
		MaterialParameter* param = new MaterialParameter();
		param->name = name;
		param->var = variable;
		this->parametersByName.Add(name, param);
		parameters.Append(param);
	}
}

} // namespace Render
