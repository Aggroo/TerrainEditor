#pragma once
#include "foundation/util/array.h"
#include "meshresource.h"
#include "shaderobject.h"

struct aiMesh;
struct aiNode;
struct aiScene;


namespace Render
{
class Model : public Core::RefCounted
{
__DeclareClass(Model)
public:
	Model();
	~Model();

	void LoadModel(Util::String path);

	void Draw();

private:
	Util::Array<Ptr<MeshResources>> meshes;
	Util::String directory;

	void ProcessNode(aiNode *node, const aiScene *scene);
	Ptr<MeshResources> ProcessMesh(aiMesh *mesh, const aiScene *scene);
};
}
