#pragma once
#include "core/singleton.h"
#include "core/refcounted.h"
#include "render/resources/material.h"
#include "render/resources/textureresource.h"
#include "render/resources/model.h"

namespace Render
{

class ResourceServer
{
__DeclareSingleton(ResourceServer)

public:
	///Returns a texture if it exists, otherwise loads it and then returns the new textureResource
	Ptr<TextureResource> LoadTexture(const Util::String& filepath);
	Ptr<TextureResource> LoadTexture(const CreateTextureParameters & textureParams);
	///Check if Texture is loaded
	bool HasTextureNamed(const Util::String& name) const;

	///Returns a mesh if it exists, otherwise loads it and then returns the new meshResource
	Ptr<MeshResources> LoadMesh(const Util::String& filepath);
	///Check if mesh is loaded
	bool HasMeshNamed(const Util::String& name) const;

	///Returns a surface if it exists, otherwise load it and then return the new Surface
	Ptr<Surface> LoadSurface(const Util::String& filepath);
	///Check if surface is loaded
	bool HasSurfaceNamed(const Util::String& name) const;

	///Returns a model if it exists, otherwise load it and then return the new model
	Ptr<Model> LoadModel(const Util::String& filepath);
	///Check if model is loaded
	bool HasModelNamed(const Util::String& name) const;

	///Loads a material .json file and adds all materials to the list if they're not already defined
	Ptr<Material> GetMaterial(const Util::String& name);
	bool SetupMaterials(const Util::String& filepath);
	bool HasMaterialNamed(const Util::String& name);

private:

	///This contains all textures.
	///Key must be unique to each texture. the key is the texture name loaded
	Util::Dictionary<Util::String, Ptr<TextureResource>> textures;

	///This contains all meshes.
	///Key must be unique to each mesh. the key is the mesh name loaded
	Util::Dictionary<Util::String, Ptr<MeshResources>> meshes;

	///Contains all Materials.
	///Key must be unique to each Material. the key is the material name
	Util::Dictionary<Util::String, Ptr<Material>> materials;

	///Contains all Surfaces.
	///Key must be unique to each Surface. the key is the Surface name
	Util::Dictionary<Util::String, Ptr<Surface>> surfaces;

	///This contains all Models currently in-game.
	Util::Dictionary<Util::String, Ptr<Model>> models;

};	
}
