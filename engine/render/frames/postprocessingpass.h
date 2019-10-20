#pragma once
#include "core/refcounted.h"
#include "drawpass.h"
#include "render/resources/texturenode.h"

namespace Render
{
class PostProcessingPass : public DrawPass
{
__DeclareClass(PostProcessingPass)
private:
	struct PostProcessOptions
	{
		float tonemapping;
		float exposure;
		float gamma;
	};

	///uniform buffer object
	GLuint ubo[1];
public:
	PostProcessingPass();
	~PostProcessingPass();

	void Setup();

	void Execute();

	void UpdateResolution();

	void UpdateUBOValues();

private:

	///Shader Settings
	PostProcessOptions postProcessVar;

	Ptr<MeshResources> postProcessingQuad;
	Ptr<ShaderObject> postProcessingShader;

};
}
