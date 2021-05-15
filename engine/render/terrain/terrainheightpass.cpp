//------------------------------------------------------------------------------
//  terrainheightpass.cpp
//  (C) 2020 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "terrainheightpass.h"
#include "render/resources/texturenode.h"
#include "render/server/shaderserver.h"
#include "render/resources/shaderobject.h"

namespace Terrain
{
__ImplementClass(Terrain::TerrainHeightPass, 'TEHP', Core::RefCounted);


TerrainHeightPass::TerrainHeightPass()
{
	glGenBuffers(1, this->ubo);
	this->heightmaps = Render::TextureNode::Create();
	this->mergedHeightmap = Render::TextureResource::Create();
}

TerrainHeightPass::~TerrainHeightPass()
{
}

void TerrainHeightPass::Setup(Ptr<Render::TextureResource> baseMap)
{
	T_CORE_TRACE("TERRAIN HEIGHT PASS Setup Terrain Height Pass");

	Ptr<Render::ShaderObject> heightmapShader = Render::ShaderServer::Instance()->GetShader("heightmap");
	this->heightmapProgram = heightmapShader->GetProgram();

	heightmapShader->BindProgram();
	heightmapShader->setupUniformInt("heightmaps[0]", (GLuint)Render::TextureIndex::heightmap1);
	heightmapShader->setupUniformInt("heightmaps[1]", (GLuint)Render::TextureIndex::heightmap2);
	heightmapShader->setupUniformInt("heightmaps[2]", (GLuint)Render::TextureIndex::heightmap3);
	heightmapShader->setupUniformInt("heightmaps[3]", (GLuint)Render::TextureIndex::heightmap4);
	heightmapShader->setupUniformInt("heightmaps[4]", (GLuint)Render::TextureIndex::heightmap5);

	this->mergedHeightmap->GetWidth() = baseMap->GetWidth();
	this->mergedHeightmap->GetHeight() = baseMap->GetHeight();
	this->mergedHeightmap->GetChannels() = baseMap->GetChannels();
	this->mergedHeightmap->textureParameters = baseMap->textureParameters;

	float borderColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	glCreateTextures(GL_TEXTURE_2D, 1, &this->mergedHeightmap->GetTextureID());
	glTextureStorage2D(this->mergedHeightmap->GetTextureID(), 1, GL_RGBA16F, baseMap->GetWidth(), baseMap->GetHeight());
	glTextureParameteri(this->mergedHeightmap->GetTextureID(), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTextureParameteri(this->mergedHeightmap->GetTextureID(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(this->mergedHeightmap->GetTextureID(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(this->mergedHeightmap->GetTextureID(), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	workGroupsX = (GLuint)(this->mergedHeightmap->GetWidth() + (this->mergedHeightmap->GetWidth() % 16)) / 16;
	workGroupsY = (GLuint)(this->mergedHeightmap->GetHeight() + (this->mergedHeightmap->GetHeight() % 16)) / 16;

	this->heightmaps->AddTexture(Render::TextureIndex::heightmap1, baseMap);
	this->heightmaps->AddTexture(Render::TextureIndex::heightmap2, baseMap);
	this->heightmaps->AddTexture(Render::TextureIndex::heightmap3, baseMap);
	this->heightmaps->AddTexture(Render::TextureIndex::heightmap4, baseMap);
	this->heightmaps->AddTexture(Render::TextureIndex::heightmap5, baseMap);

}

void TerrainHeightPass::Execute()
{
	glUseProgram(this->heightmapProgram);

	//Bind uniform buffer block
	glBindBuffer(GL_UNIFORM_BUFFER, this->ubo[0]);
	glBindBufferBase(GL_UNIFORM_BUFFER, 14, this->ubo[0]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(LayerVariables), &layerVars, GL_STATIC_DRAW);

	this->heightmaps->BindTextures();

	glBindImageTexture(1, this->mergedHeightmap->GetTextureID(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA16F);
	
	glDispatchCompute(workGroupsX, workGroupsY, 1);

	this->heightmaps->UnbindTextures();

}

void TerrainHeightPass::UpdateTexture(Render::TextureIndex index, const char * filename)
{
	T_CORE_TRACE("TERRAIN HEIGHT PASS Update Terrain Height Textures");
	this->heightmaps->UpdateTexture(index, filename);
	Execute();
}

} // namespace Terrain
