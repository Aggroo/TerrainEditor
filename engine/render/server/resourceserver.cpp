#include "config.h"
#include "resourceserver.h"
#include <fstream>
#include "render/resources/surface.h"
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
bool ResourceServer::HasTextureNamed(const Util::String& name) const
{
	return this->textures.Contains(name);
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
bool ResourceServer::HasMeshNamed(const Util::String& name) const
{
	return this->meshes.Contains(name);
}

Ptr<Surface> ResourceServer::LoadSurface(const Util::String & filepath)
{
	std::ifstream i(filepath.AsCharPtr());

	if (!i) {
		printf("[SHADER LOAD ERROR]: Couldn't find shaders.json!");
		_assert(false);
		return false;
	}

	nlohmann::json j;
	i >> j;

	auto surface = j.at("Surface").get<Components::Surface>();

	if (!this->HasMaterialNamed(surface.material.c_str()))
	{
		_warning("Duplicate shader loaded: \" %s \". Using previously loaded shader...", surface.material.c_str());
	}
	else
	{
		Ptr<Render::Surface> sur = Surface::Create();

		sur->material = this->GetMaterial(surface.material.c_str());

		sur->name = filepath.ExtractFileName().AsCharPtr();
		sur->filepath = filepath.AsCharPtr();

		for (auto parameter : surface.param)
		{
			Util::Variant var = Util::Variant::FromString(parameter.value.c_str());

			sur->AddParameter(parameter.name.c_str(), var);
		}

		sur->SetupShaderUniforms();

		this->surfaces.Add(filepath.ExtractFileName().AsCharPtr(), sur);

		sur->material->surfaces.Append(sur);

		return sur;
	}
	
	return nullptr;
}

bool ResourceServer::HasSurfaceNamed(const Util::String & name) const
{
	return this->surfaces.Contains(name);
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

bool ResourceServer::SetupMaterials(const Util::String& filepath)
{
	std::ifstream i(filepath.AsCharPtr());

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
				Util::Variant var = Util::Variant::FromString(parameter.value.c_str());
				
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
