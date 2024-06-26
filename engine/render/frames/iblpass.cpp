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
	hdrTexPath = "resources/textures/skyboxes/qwantani_2k.hdr";
}

IBLPass::~IBLPass()
{
}

void IBLPass::Setup()
{
	T_CORE_TRACE("IBL PASS Generating pass");

	

	ComputeShaderIBL();

	//FrameBufferIBL();

	FramePass::Setup();
}

void IBLPass::ComputeShaderIBL()
{
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
		Ptr<ShaderObject> eq2Cubemap = Render::ShaderServer::Instance()->GetShader("eq2cube-cs");

		Ptr<TextureResource> hdr = TextureResource::Create();
		hdr->LoadTextureFile(hdrTexPath.AsCharPtr());

		Ptr<TextureResource> envTexEq = TextureResource::Create();
		glCreateTextures(GL_TEXTURE_2D, 1, &envTexEq->GetTextureID());
		//int levels = numMipMapLevels(hdr->GetWidth(), hdr->GetHeight());
		glTextureStorage2D(envTexEq->GetTextureID(), 1, GL_RGB16F, hdr->GetWidth(), hdr->GetHeight());
		glTextureParameteri(envTexEq->GetTextureID(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(envTexEq->GetTextureID(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameterf(envTexEq->GetTextureID(), GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
		if (hdr->IsHDR())
			glTextureSubImage2D(envTexEq->GetTextureID(), 0, 0, 0, hdr->GetWidth(), hdr->GetHeight(), GL_RGB, GL_FLOAT, hdr->pixels<float>());
		else
			glTextureSubImage2D(envTexEq->GetTextureID(), 0, 0, 0, hdr->GetWidth(), hdr->GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, hdr->pixels<unsigned char>());


		eq2Cubemap->BindProgram();
		glBindTextureUnit(0, envTexEq->GetTextureID());
		glBindImageTexture(0, envMap->GetTextureID(), 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA16F);
		glDispatchCompute(kEnvMapSize / 32, kEnvMapSize / 32, 6);

		glDeleteTextures(1, &envTexEq->GetTextureID());
		//glDeleteProgram(eq2Cubemap->GetProgram());

	}

	if (mipLevels > 1)
	{
		glGenerateTextureMipmap(envMap->GetTextureID());
	}

	{
		Ptr<ShaderObject> spmap = Render::ShaderServer::Instance()->GetShader("spmap-cs");

		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &sp->GetTextureID());
		int levels = numMipMapLevels(kEnvMapSize, kEnvMapSize);
		glTextureStorage2D(sp->GetTextureID(), levels, GL_RGBA16F, kEnvMapSize, kEnvMapSize);
		if (levels > 1)
		{
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
		const float deltaRoughness = 1.0f / Math::Max(float(levels - 1), 1.0f);
		for (int level = 1, size = kEnvMapSize / 2; level <= levels; ++level, size /= 2)
		{
			const GLuint numGroups = Math::Max(1, size / 32);
			glBindImageTexture(0, sp->GetTextureID(), level, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA16F);
			glProgramUniform1f(spmap->GetProgram(), 0, level * deltaRoughness);
			glDispatchCompute(numGroups, numGroups, 6);
		}
		//glDeleteProgram(spmap->GetProgram());
	}

	{
		Ptr<ShaderObject> irmap = Render::ShaderServer::Instance()->GetShader("irmap-cs");

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
		//glDeleteProgram(irmap->GetProgram());
	}

	{
		Ptr<ShaderObject> brdfmap = Render::ShaderServer::Instance()->GetShader("brdf-cs");

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
		//glDeleteProgram(brdfmap->GetProgram());
	}
}

void IBLPass::FrameBufferIBL()
{
	static constexpr int kEnvMapSize = 1024;
	static constexpr int kIrradianceMapSize = 32;
	static constexpr int kBRDF_LUT_Size = 256;

	float maxAnisotropy;
	Ptr<Skybox> skyboxCube = Skybox::Create();

	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);

	Ptr<TextureResource> envMap = TextureResource::Create();
	glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &envMap->GetTextureID());
	int mipLevels = numMipMapLevels(kEnvMapSize, kEnvMapSize);
	glTextureStorage2D(envMap->GetTextureID(), mipLevels, GL_RGBA16F, kEnvMapSize, kEnvMapSize);
	glTextureParameteri(envMap->GetTextureID(), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTextureParameteri(envMap->GetTextureID(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameterf(envMap->GetTextureID(), GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);

	GLuint captureFBO;
	GLuint captureRBO;
	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);


	Math::mat4 captureProjection = Math::mat4::perspectiveMatrix(Math::Deg2Rad(90.0f), 1.0f, 0.1f, 10.0f);
	Math::mat4 captureViews[] =
	{
		Math::mat4::LookAt(Math::vec4(0.0f, 0.0f, 0.0f), Math::vec4(1.0f,  0.0f,  0.0f), Math::vec4(0.0f, -1.0f,  0.0f)),
		Math::mat4::LookAt(Math::vec4(0.0f, 0.0f, 0.0f), Math::vec4(-1.0f,  0.0f,  0.0f), Math::vec4(0.0f, -1.0f,  0.0f)),
		Math::mat4::LookAt(Math::vec4(0.0f, 0.0f, 0.0f), Math::vec4(0.0f,  1.0f,  0.0f), Math::vec4(0.0f,  0.0f,  1.0f)),
		Math::mat4::LookAt(Math::vec4(0.0f, 0.0f, 0.0f), Math::vec4(0.0f, -1.0f,  0.0f), Math::vec4(0.0f,  0.0f, -1.0f)),
		Math::mat4::LookAt(Math::vec4(0.0f, 0.0f, 0.0f), Math::vec4(0.0f,  0.0f,  1.0f), Math::vec4(0.0f, -1.0f,  0.0f)),
		Math::mat4::LookAt(Math::vec4(0.0f, 0.0f, 0.0f), Math::vec4(0.0f,  0.0f, -1.0f), Math::vec4(0.0f, -1.0f,  0.0f))
	};

	{
		Ptr<ShaderObject> eq2Cubemap = Render::ShaderServer::Instance()->GetShader("eq2cube");

		Ptr<TextureResource> hdr = TextureResource::Create();
		hdr->LoadTextureFile(hdrTexPath.AsCharPtr());

		eq2Cubemap->BindProgram();
		eq2Cubemap->setupUniformInt("equirectangularMap", 0);
		eq2Cubemap->setupMatrix4fv("Projection", captureProjection);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, hdr->GetTextureID());

		glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		for (unsigned int i = 0; i < 6; ++i)
		{
			eq2Cubemap->setupMatrix3fv("View", Math::mat3::fromMatrix4D(captureViews[i]));
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envMap->GetTextureID(), 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			skyboxCube->DrawCube();
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//glDeleteProgram(eq2Cubemap->GetProgram());

	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, envMap->GetTextureID());
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	{
		Ptr<ShaderObject> irmap = Render::ShaderServer::Instance()->GetShader("irmap");

		glGenTextures(1, &ir->GetTextureID());
		glBindTexture(GL_TEXTURE_CUBE_MAP, ir->GetTextureID());
		//int levels = numMipMapLevels(kIrradianceMapSize, kIrradianceMapSize);
		for (unsigned int i = 0; i < 6; ++i)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

		// pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
		// -----------------------------------------------------------------------------
		irmap->BindProgram();
		irmap->setupUniformInt("environmentMap", 0);
		irmap->setupMatrix4fv("Projection", captureProjection);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, envMap->GetTextureID());

		glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		for (unsigned int i = 0; i < 6; ++i)
		{
			irmap->setupMatrix3fv("view", Math::mat3::fromMatrix4D(captureViews[i]));
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, ir->GetTextureID(), 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			skyboxCube->DrawCube();
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//glDeleteProgram(irmap->GetProgram());
	}

	{
		Ptr<ShaderObject> spmap = Render::ShaderServer::Instance()->GetShader("spmap");

		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &sp->GetTextureID());
		int levels = numMipMapLevels(kEnvMapSize, kEnvMapSize);
		glTextureStorage2D(sp->GetTextureID(), levels, GL_RGBA16F, kEnvMapSize, kEnvMapSize);
		if (levels > 1)
		{
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
		const float deltaRoughness = 1.0f / Math::Max(float(levels - 1), 1.0f);
		for (int level = 1, size = kEnvMapSize / 2; level <= levels; ++level, size /= 2)
		{
			const GLuint numGroups = Math::Max(1, size / 32);
			glBindImageTexture(0, sp->GetTextureID(), level, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA16F);
			glProgramUniform1f(spmap->GetProgram(), 0, level * deltaRoughness);
			glDispatchCompute(numGroups, numGroups, 6);
		}
		//glDeleteProgram(spmap->GetProgram());
	}

	{
		Ptr<ShaderObject> brdfmap = Render::ShaderServer::Instance()->GetShader("brdf");

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
		//glDeleteProgram(brdfmap->GetProgram());
	}
}

void IBLPass::Execute()
{
	FramePass::Execute();
}

void IBLPass::UpdateResolution()
{
}

void IBLPass::UpdateIBL(Util::String newTexturePath)
{
	hdrTexPath = newTexturePath;
	Setup();
}
}



