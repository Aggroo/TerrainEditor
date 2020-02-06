#pragma once
#include "drawpass.h"

namespace Render
{

class SSAOPass : public DrawPass
{
__DeclareClass(SSAOPass)
public:
	SSAOPass();
	~SSAOPass();

	void Setup();

	void Execute();

	void UpdateResolution();

	Ptr<TextureResource> GetSSAOBuffer();

private:

	Ptr<TextureResource> ssaoColorBufferBlur, noiseTexture, ssaoColorBuffer, defaultAO;

	//The FBO of the Pass
	GLuint ssaoFBO, ssaoBlurFBO;

	//Random Kernel rotations
	Util::Array<Math::vec3> ssaoNoise;

	//SSAO sample kernels
	Util::Array<Math::vec3> ssaoKernel;

	Ptr<MeshResources> ssaoQuad;
	Ptr<ShaderObject> ssaoShader;
	Ptr<ShaderObject> ssaoBlurShader;
};

}