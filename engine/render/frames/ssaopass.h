#pragma once
#include "drawpass.h"

namespace Render
{

class SSAOPass : public DrawPass
{
__DeclareClass(SSAOPass)
private:
	struct SSAOOptions
	{
		int kernel;
		float radius;
		float bias;
	};

	///uniform buffer object
	GLuint ubo[1];
public:
	SSAOPass();
	~SSAOPass();

	void Setup();

	void Execute();

	void UpdateResolution();

	Ptr<TextureResource> GetSSAOBuffer();

	void UpdateUBOValues();

private:

	Ptr<TextureResource> ssaoColorBufferBlur, noiseTexture, ssaoColorBuffer, defaultAO;

	///Shader Settings
	SSAOOptions ssaoVar;

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