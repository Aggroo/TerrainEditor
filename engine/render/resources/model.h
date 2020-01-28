#pragma once
#include "foundation/util/array.h"
#include "meshresource.h"
#include "texturenode.h"
#include "shaderobject.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace Render
{
class Model : public Core::RefCounted
{
__DeclareClass(Model)
public:
	Model();
	~Model();

	void LoadModel(Util::String path);
	void LoadMaterial(aiMaterial* mat, aiTextureType type, int meshIndex);

	void Draw();

private:
	Util::Array<Ptr<MeshResources>> meshes;
	Util::Array<Ptr<TextureNode>> textures;
	Util::String directory;

	void ProcessNode(aiNode *node, const aiScene *scene);
	Ptr<MeshResources> ProcessMesh(aiMesh *mesh, const aiScene *scene);
};
}
