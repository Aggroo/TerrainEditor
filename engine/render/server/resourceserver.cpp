#include "config.h"
#include "resourceserver.h"
#include <fstream>
#include "render/resources/materialcomponents.h"

namespace Render
{
//------------------------------------------------------------------------------
/**
*/
ResourceServer::ResourceServer()
{

}
//------------------------------------------------------------------------------
/**
*/
Ptr<TextureResource> ResourceServer::LoadTexture(const Util::String& filepath)
{
	if(!this->HasTextureNamed(filepath))
	{
		Ptr<TextureResource> texture = TextureResource::Create();
		texture->LoadTextureFile(filepath.AsCharPtr());
		this->textures.Add(filepath, texture);

		return texture;
	}
	else
	{
		return this->textures[filepath];
	}
}


//------------------------------------------------------------------------------
/**
*/
bool ResourceServer::HasTextureNamed(const Util::String& nName) const
{
	return this->textures.Contains(nName); 
}

//------------------------------------------------------------------------------
/**
*/
Ptr<MeshResources> ResourceServer::LoadMesh(const Util::String& filepath)
{
	if (!this->HasMeshNamed(filepath))
	{
		Ptr<MeshResources> mesh = MeshResources::Create();
		mesh->LoadMesh(filepath.AsCharPtr());
		this->meshes.Add(filepath, mesh);

		return mesh;
	}
	else
	{
		return this->meshes[filepath];
	}
}

//------------------------------------------------------------------------------
/**
*/
bool ResourceServer::HasMeshNamed(const Util::String& nName) const
{
	return this->meshes.Contains(nName);
}

Ptr<Material> ResourceServer::GetMaterial(const Util::String& name)
{
	if (this->HasMaterialNamed(name))
	{
		return this->materials[name];
	}
	else
	{
		printf("[MATERIAL ERROR]: Could not retrieve material!");
		_assert(false);
		return nullptr;
	}
}

bool ResourceServer::SetupMaterials(const Util::String& fileName)
{
	std::ifstream i(fileName.AsCharPtr());

	if (!i) {
		printf("[SHADER LOAD ERROR]: Couldn't find shaders.json!");
		_assert(false);
		return false;
	}

	nlohmann::json j;
	i >> j;

	this->materials.BeginBulkAdd();

	for (auto& element : j.at("Materials"))
	{
		auto material = element.get<Components::Materials>();

		if (this->HasMaterialNamed(material.name.c_str()))
		{
			_warning("Duplicate shader loaded: \" %s \". Using previously loaded shader...", material.name.c_str());
		}
		else
		{
			Ptr<Render::Material> mat = Material::Create();
			mat->SetName(material.name.c_str());
			
			for (auto pass : material.pass)
			{
				mat->SetFramePass(pass.name.c_str(), pass.shader.c_str());
			}

			for (auto parameter : material.parameter)
			{
				Util::Variant var = Util::Variant::FromString(parameter.defaultValue.c_str());
				
				mat->AddParameter(parameter.name.c_str(), var);
			}

			this->materials.Add(material.name.c_str(), mat);
		}
	}

	this->materials.EndBulkAdd();

	return true;
}

bool ResourceServer::HasMaterialNamed(const Util::String& name)
{
	return this->materials.Contains(name);
}

}
