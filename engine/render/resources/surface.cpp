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
#include "render/server/frameserver.h"
#include "render/render/renderer.h"

namespace Render
{
__ImplementClass(Render::Surface, 'SRFC', Core::RefCounted);

Surface::Surface()
{
	this->textures = Render::TextureNode::Create();
	glGenBuffers(1, this->ubo);
	uniformBufferExist = false;
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
			case Util::Variant::Type::Bool:
				if (param->name == "IBL" && param->var.GetBool())
				{
					shaders[i]->setupUniformInt("environmentMap", (GLuint)Render::TextureIndex::environmentMap);
					shaders[i]->setupUniformInt("irradianceMap", (GLuint)Render::TextureIndex::irradiance);
					shaders[i]->setupUniformInt("brdfLUT", (GLuint)Render::TextureIndex::brdf);
					this->textures->AddTexture(Render::TextureIndex::environmentMap, Render::FrameServer::Instance()->GetIBLPass()->GetEnvironmentMap());
					this->textures->AddTexture(Render::TextureIndex::irradiance, Render::FrameServer::Instance()->GetIBLPass()->GetIrradianceMap());
					this->textures->AddTexture(Render::TextureIndex::brdf, Render::FrameServer::Instance()->GetIBLPass()->GetBRDFMap());
				}
				break;
			default:
				T_CORE_ERROR("ERROR: Parameter might not be fully implemented!");
				assert(false);
				break;
			}
		}
	}
}

void Surface::BindUniformBuffer()
{
	if (this->uniformBufferExist)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, this->ubo[0]);
		glBindBufferBase(GL_UNIFORM_BUFFER, 1, this->ubo[0]);
		glBufferData(GL_UNIFORM_BUFFER, uboBuffer->bufferSize, uboBuffer->buffer, GL_STATIC_DRAW);
	}
	
}

SamplerParameters Surface::SetupTextureSampler(const Util::String& sampler)
{
	SamplerParameters samplerParam;
	if (sampler == "ClampBorderBilinear")
	{
		samplerParam.minFilter = TextureFilterMode::LinearMipmapLinear;
		samplerParam.maxFilter = TextureFilterMode::Linear;
		samplerParam.wrapU = TextureWrapMode::ClampToBorder;
		samplerParam.wrapV = TextureWrapMode::ClampToBorder;
	}
	else if (sampler == "ClampBorderPoint")
	{
		samplerParam.minFilter = TextureFilterMode::NearestMipmapNearest;
		samplerParam.maxFilter = TextureFilterMode::Nearest;
		samplerParam.wrapU = TextureWrapMode::ClampToBorder;
		samplerParam.wrapV = TextureWrapMode::ClampToBorder;
	}
	else if (sampler == "ClampEdgeBilinear")
	{
		samplerParam.minFilter = TextureFilterMode::LinearMipmapLinear;
		samplerParam.maxFilter = TextureFilterMode::Linear;
		samplerParam.wrapU = TextureWrapMode::ClampToEdge;
		samplerParam.wrapV = TextureWrapMode::ClampToEdge;
	}
	else if(sampler == "ClampEdgePoint")
	{ 
		samplerParam.minFilter = TextureFilterMode::NearestMipmapNearest;
		samplerParam.maxFilter = TextureFilterMode::Nearest;
		samplerParam.wrapU = TextureWrapMode::ClampToEdge;
		samplerParam.wrapV = TextureWrapMode::ClampToEdge;
	}
	else if(sampler == "RepeatBilinear")
	{ 
		samplerParam.minFilter = TextureFilterMode::LinearMipmapLinear;
		samplerParam.maxFilter = TextureFilterMode::Linear;
		samplerParam.wrapU = TextureWrapMode::Repeat;
		samplerParam.wrapV = TextureWrapMode::Repeat;
	}
	else if (sampler == "RepeatPoint")
	{
		samplerParam.minFilter = TextureFilterMode::NearestMipmapNearest;
		samplerParam.maxFilter = TextureFilterMode::Nearest;
		samplerParam.wrapU = TextureWrapMode::Repeat;
		samplerParam.wrapV = TextureWrapMode::Repeat;
	}
	else
	{
		T_CORE_ERROR("SAMPLER ERROR: The sampler name that was used does not exist");
		_assert(false);
	}
	return samplerParam;
}

Render::MaterialParameter* Surface::GetParameterByName(const Util::String & name)
{
	return this->parametersByName[name];
}

void Surface::UpdateParameterByName(const Util::String & name, const Util::Variant & newValue)
{
	Render::MaterialParameter* param = this->parametersByName[name];

	int findIndex = this->parameters.FindIndex(param);
	if (findIndex != -1 && param->var.GetType() == newValue.GetType())
	{
		param->var = newValue;
		this->parameters[findIndex]->var = newValue;
	}
}

void Surface::AddModelNode(ModelNode* node)
{
	this->modelNodes.Append(node);
}

bool Surface::RemoveModelNode(ModelNode* node)
{
	auto n = this->modelNodes.Find(node);
	if (n != nullptr)
	{
		this->modelNodes.Remove(n);
		return true;
	}
	return false;
}

const Util::Array<ModelNode*>& Surface::GetModelNodes()
{
	return this->modelNodes;
}

void Surface::SetUniformBuffer(Render::UBOInfo* buffer)
{
	this->uboBuffer = buffer;
	uniformBufferExist = true;
}

void Surface::AddTexture(const Util::String& name, const Util::Variant & variable, const Util::String& textureSample)
{

	CreateTextureParameters textureParam;
	textureParam.filename = variable.GetString().AsCharPtr();
	textureParam.sampler = SetupTextureSampler(textureSample);

	this->textures->AddTexture(Material::TextureIndexFromString(name.AsCharPtr()), textureParam);

	MaterialParameter* param = new MaterialParameter();
	param->name = name;
	param->var = variable;
	this->parametersByName.Add(name, param);
	parameters.Append(param);

}

void Surface::AddParameter(const Util::String & name, const Util::Variant & variable)
{
	MaterialParameter* param = new MaterialParameter();
	param->name = name;
	param->var = variable;
	this->parametersByName.Add(name, param);
	parameters.Append(param);
}

} // namespace Render
