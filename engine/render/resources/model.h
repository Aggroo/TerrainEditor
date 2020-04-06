#pragma once
#include "core/refcounted.h"
#include "foundation/util/array.h"

namespace Render
{

class MeshResources;
class Material;
class Surface;
class Model;

struct ModelNode
{
	~ModelNode();
	//index for the primitivegroup to render
	uint primitiveGroup;
	//pointer back to the surface
	Surface* surface;
	//pointer to the modelinstance that own this modelnode
	Model* modelInstance;
};

class Model : public Core::RefCounted
{
__DeclareClass(Model)
public:
	Model();
	~Model();

	Ptr<MeshResources> GetMesh();

	const Util::Array<ModelNode*>& GetModelNodes() { return this->nodes; }

	void AddModelNode(ModelNode* node) { nodes.Append(node); }

	Math::mat4 transform;

private:

	friend class ResourceServer;

	//Loaded mesh with primitives
	Ptr<MeshResources> mesh;
	//A list of model nodes with a surface and a index to it's primitive group
	Util::Array<ModelNode*> nodes;


	Util::String directory;
};
}
