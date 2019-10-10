#include "config.h"
#include "iblpass.h"
#include "render/server/shaderserver.h"
#include "foundation/math/math.h"

namespace Render
{
__ImplementClass(Render::IBLPass, 'REIB', Render::DrawPass)
IBLPass::IBLPass()
{
	ir = TextureResource::Create();
	brdf = TextureResource::Create();
	sp = TextureResource::Create();
}

IBLPass::~IBLPass()
{
}

void IBLPass::Setup()
{
	// Parameters
	static constexpr int kEnvMapSize = 1024;
	static constexpr int kIrradianceMapSize = 32;
	static constexpr int kBRDF_LUT_Size = 256;
	float maxAnisotropy;

	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);

	Ptr<TextureResource> envMap = TextureResource::Create();
	glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &envMap->GetTextureID());
	int mipLevels = numMipMapLevels(kEnvMapSize, kEnvMapSize);
	glTextureStorage2D(envMap->GetTextureID(), mipLevels, GL_RGBA16F, kEnvMapSize, kEnvMapSize);
	glTextureParameteri(envMap->GetTextureID(), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTextureParameteri(envMap->GetTextureID(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameterf(envMap->GetTextureID(), GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);

	{
		Ptr<ShaderObject> eq2Cubemap = ShaderObject::Create();
		GLuint comp = Render::ShaderServer::Instance()->LoadComputeShader("resources/shaders/eq2cube.comp");
		eq2Cubemap->AddShader(comp);
		eq2Cubemap->LinkShaders();
		Render::ShaderServer::Instance()->AddShaderObject("EQ2CUBE", eq2Cubemap);

		Ptr<TextureResource> hdr = TextureResource::Create();
		hdr->LoadTextureFile("resources/textures/skyboxes/hdri_sky_01_full.hdr");

		Ptr<TextureResource> envTexEq = TextureResource::Create();
		glCreateTextures(GL_TEXTURE_2D, 1, &envTexEq->GetTextureID());
		//int levels = numMipMapLevels(hdr->GetWidth(), hdr->GetHeight());
		glTextureStorage2D(envTexEq->GetTextureID(), 1, GL_RGB16F, hdr->GetWidth(), hdr->GetHeight());
		glTextureParameteri(envTexEq->GetTextureID(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(envTexEq->GetTextureID(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameterf(envTexEq->GetTextureID(), GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
		if(hdr->IsHDR())
			glTextureSubImage2D(envTexEq->GetTextureID(), 0, 0, 0, hdr->GetWidth(), hdr->GetHeight(), GL_RGB, GL_FLOAT, hdr->pixels<float>());
		else
			glTextureSubImage2D(envTexEq->GetTextureID(), 0, 0, 0, hdr->GetWidth(), hdr->GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, hdr->pixels<unsigned char>());


		eq2Cubemap->BindProgram();
		glBindTextureUnit(0, envTexEq->GetTextureID());
		glBindImageTexture(0, envMap->GetTextureID(), 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA16F);
		glDispatchCompute(kEnvMapSize / 32, kEnvMapSize / 32, 6);

		glDeleteTextures(1, &envTexEq->GetTextureID());
		glDeleteProgram(eq2Cubemap->GetProgram());

	}

	if (mipLevels > 1) {
		glGenerateTextureMipmap(envMap->GetTextureID());
	}

	{
		Ptr<ShaderObject> spmap = ShaderObject::Create();
		GLuint comp = Render::ShaderServer::Instance()->LoadComputeShader("resources/shaders/spmap.comp");
		spmap->AddShader(comp);
		spmap->LinkShaders();
		Render::ShaderServer::Instance()->AddShaderObject("SP", spmap);
		

		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &sp->GetTextureID());
		int levels = numMipMapLevels(kEnvMapSize, kEnvMapSize);
		glTextureStorage2D(sp->GetTextureID(), levels, GL_RGBA16F, kEnvMapSize, kEnvMapSize);
		if (levels > 1) {
			glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		}
		glTextureParameteri(sp->GetTextureID(), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureParameteri(sp->GetTextureID(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameterf(sp->GetTextureID(), GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
		

		glCopyImageSubData(envMap->GetTextureID(), GL_TEXTURE_CUBE_MAP, 0, 0, 0, 0,
			sp->GetTextureID(), GL_TEXTURE_CUBE_MAP, 0, 0, 0, 0,
			kEnvMapSize, kEnvMapSize, 6);


		spmap->BindProgram();
		glBindTextureUnit(0, envMap->GetTextureID());

		// Pre-filter rest of the mip chain.
		const float deltaRoughness = 1.0f / Math::max(float(levels - 1), 1.0f);
		for (int level = 1, size = kEnvMapSize / 2; level <= levels; ++level, size /= 2) {
			const GLuint numGroups = Math::max(1, size / 32);
			glBindImageTexture(0, sp->GetTextureID(), level, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA16F);
			glProgramUniform1f(spmap->GetProgram(), 0, level * deltaRoughness);
			glDispatchCompute(numGroups, numGroups, 6);
		}
		glDeleteProgram(spmap->GetProgram());
	}
	
	{
		Ptr<ShaderObject> irmap = ShaderObject::Create();
		GLuint comp = Render::ShaderServer::Instance()->LoadComputeShader("resources/shaders/irmap.comp");
		irmap->AddShader(comp);
		irmap->LinkShaders();
		Render::ShaderServer::Instance()->AddShaderObject("IR", irmap);
		irmap->BindProgram();

		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &ir->GetTextureID());
		//int levels = numMipMapLevels(kIrradianceMapSize, kIrradianceMapSize);
		glTextureStorage2D(ir->GetTextureID(), 1, GL_RGBA16F, kIrradianceMapSize, kIrradianceMapSize);
		glTextureParameteri(ir->GetTextureID(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(ir->GetTextureID(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameterf(ir->GetTextureID(), GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);

		irmap->BindProgram();
		glBindTextureUnit(0, sp->GetTextureID());
		glBindImageTexture(0, ir->GetTextureID(), 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA16F);
		glDispatchCompute(kIrradianceMapSize / 32, kIrradianceMapSize / 32, 6);
		glDeleteProgram(irmap->GetProgram());
	}
	
	{
		Ptr<ShaderObject> brdfmap = ShaderObject::Create();
		GLuint comp = Render::ShaderServer::Instance()->LoadComputeShader("resources/shaders/brdf.comp");
		brdfmap->AddShader(comp);
		brdfmap->LinkShaders();
		Render::ShaderServer::Instance()->AddShaderObject("BRDF", brdfmap);
		

		glCreateTextures(GL_TEXTURE_2D, 1, &brdf->GetTextureID());
		glTextureStorage2D(brdf->GetTextureID(), 1, GL_RG16F, kBRDF_LUT_Size, kBRDF_LUT_Size);
		glTextureParameteri(brdf->GetTextureID(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(brdf->GetTextureID(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameterf(brdf->GetTextureID(), GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);

		glTextureParameteri(brdf->GetTextureID(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(brdf->GetTextureID(), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		brdfmap->BindProgram();
		glBindImageTexture(0, brdf->GetTextureID(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RG16F);
		glDispatchCompute(kBRDF_LUT_Size / 32, kBRDF_LUT_Size / 32, 1);
		glDeleteProgram(brdfmap->GetProgram());
	}
	
	FramePass::Setup();
}

void IBLPass::Execute()
{
	FramePass::Execute();
}

void IBLPass::UpdateResolution()
{
}

}



