#pragma once
#include "core/refcounted.h"
#include "drawpass.h"
#include "render/resources/texturenode.h"

namespace Render
{
class PostProcessingPass : public DrawPass
{
__DeclareClass(PostProcessingPass)
public:
	PostProcessingPass();
	~PostProcessingPass();

	void Setup();

	void Execute();

	void UpdateResolution();


private:

	Ptr<MeshResources> postProcessingQuad;
	Ptr<ShaderObject> postProcessingShader;

};
}
