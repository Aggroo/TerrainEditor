#include "config.h"
#include "model.h"
#include "meshresource.h"
#include "render/server/resourceserver.h"
#include "render/resources/surface.h"

namespace Render
{
__ImplementClass(Render::Model, 'MODL', Core::RefCounted);

ModelNode::~ModelNode()
{
	if (surface != nullptr)
	{
		surface->RemoveModelNode(this);
	}
}

Model::Model()
{
	mesh = MeshResources::Create();
}

Model::~Model()
{
	for (size_t i = 0; i < this->nodes.Size(); ++i)
	{
		delete this->nodes[i];
	}
}

Ptr<MeshResources> Model::GetMesh()
{
	return this->mesh;
}

}
