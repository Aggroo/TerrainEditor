#include "config.h"
#include "perlinnoisegenerator.h"
#include "foundation/math/math.h"
#include "render/server/shaderserver.h"
#include "render/server/frameserver.h"
#include "render/window.h"


namespace Generator
{
__ImplementClass(Generator::PerlinNoiseGenerator, 'GEPN', Core::RefCounted)

PerlinNoiseGenerator::PerlinNoiseGenerator() : gen(rd()), dis(0, 3000), noiseMap(nullptr)
{
	this->texture = Render::TextureResource::Create();
	this->shader = Render::ShaderObject::Create();

	const GLuint comp = Render::ShaderServer::Instance()->LoadComputeShader("resources/shaders/perlinnoise.comp");

	this->shader->AddShader(comp);
	this->shader->LinkShaders();

	Render::ShaderServer::Instance()->AddShaderObject("PerlinNoise", this->shader);

	this->shader->BindProgram();

}

PerlinNoiseGenerator::~PerlinNoiseGenerator()
{
	delete noiseMap;
}

void PerlinNoiseGenerator::GenerateNoiseMap(const char* filename, int mapWidth, int mapHeight, float scale, int octaves, float persistance,
	float lacunarity)
{
	glUseProgram(this->shader->GetProgram());

	glGenTextures(1, &this->texture->GetTextureID());
	glBindTexture(GL_TEXTURE_2D, this->texture->GetTextureID());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, mapWidth, mapHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindTexture(GL_TEXTURE_2D, 0);

	this->shader->setupUniformFloat("time", (GLfloat)glfwGetTime());
	this->shader->setupUniformFloat("scale", scale);
	this->shader->setupUniformInt("octaves", octaves);
	this->shader->setupUniformFloat("persistance", persistance);
	this->shader->setupUniformFloat("lacunarity", lacunarity);
	this->shader->setupUniformFloat("width", mapWidth);

	//uniformBlock.pointLightCount = (GLint)LightServer::Instance()->GetNumPointLights();
	//uniformBlock.spotLightCount = (GLint)LightServer::Instance()->GetNumSpotLights();
	//
	////Bind uniform buffer block
	//glBindBuffer(GL_UNIFORM_BUFFER, this->ubo[0]);
	//glBindBufferBase(GL_UNIFORM_BUFFER, 24, this->ubo[0]);
	//glBufferData(GL_UNIFORM_BUFFER, sizeof(UniformBlock), &uniformBlock, GL_STATIC_DRAW);

	//
	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, lightServer->GetPointLightBuffer());
	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 11, lightServer->GetSpotLightBuffer());
	//
	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, lightServer->GetVisiblePointLightIndexBuffer());
	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 12, lightServer->GetVisibleSpotLightIndexBuffer());

	

	glBindImageTexture(0, texture->GetTextureID(), 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA16F);
	glDispatchCompute(mapWidth / 16, mapHeight / 16, 1);

	//noiseMap = new float[mapWidth*mapHeight];
	//
	//perlin.reseed(dis(gen));
	//
	//if(scale <= 0)
	//{
	//	scale = 0.00001f;
	//}
	//
	//float maxNoiseHeight = FLT_MIN;
	//float minNoiseHeight = FLT_MAX;
	//
	//for (int y = 0; y < mapHeight; ++y)
	//{
	//	for (int x = 0; x < mapWidth; ++x)
	//	{
	//		float amplitude = 1;
	//		float frequency = 1;
	//		float noiseHeight = 0;
	//
	//		for (int i = 0; i < octaves; ++i)
	//		{
	//			float sampleX = x / scale * frequency;
	//			float sampleY = y / scale * frequency;
	//
	//			float perlinValue = perlin.octaveNoise0_1(sampleX, sampleY, octaves) * 2 - 1;
	//			noiseHeight += (1-abs(perlinValue)) * amplitude;
	//
	//			amplitude *= persistance;
	//			frequency *= lacunarity;
	//		}
	//
	//		if(noiseHeight > maxNoiseHeight)
	//		{
	//			maxNoiseHeight = noiseHeight;
	//		}
	//		else if(noiseHeight < minNoiseHeight)
	//		{
	//			minNoiseHeight = noiseHeight;
	//		}
	//
	//		noiseMap[y * mapWidth + x] = noiseHeight;
	//	}
	//}
	//for (int y = 0; y < mapHeight; ++y)
	//{
	//	for (int x = 0; x < mapWidth; ++x)
	//	{
	//		noiseMap[y * mapWidth + x] = Math::InverseLerp(minNoiseHeight, maxNoiseHeight, noiseMap[y * mapWidth + x]);
	//	}
	//}
	//
	Color* colorMap = new Color[mapWidth * mapHeight];
	//
	//for (int y = 0; y < mapHeight; ++y)
	//{
	//	for (int x = 0; x < mapWidth; ++x)
	//	{
	//		colorMap[y * mapWidth + x] = Math::Lerp(0.0f, 1.0f, noiseMap[y * mapWidth + x]);
	//	}
	//}
	//
	glBindTexture(GL_TEXTURE_2D, this->texture->GetTextureID()); 
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, colorMap);

	//glGetTextureImage(this->texture->GetTextureID(), 0, GL_RGB, GL_FLOAT, 512*512, &colorMap);
	texture->WriteToJPG(filename, mapWidth, mapHeight, (void*)colorMap, 100);
}
}
