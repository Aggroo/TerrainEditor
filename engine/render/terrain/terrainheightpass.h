#pragma once
//------------------------------------------------------------------------------
/**
    Height pass for terrain. Runs a compute shader to blend multiple heightmaps into one

    (C) 2020 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/refcounted.h"
#include <gl/glew.h>

namespace Render
{
class TextureNode;
class TextureResource;
enum class TextureIndex;
}

namespace Terrain
{
class TerrainHeightPass : public Core::RefCounted
{
    __DeclareClass(TerrainHeightPass);
private:

	struct Layer
	{
		
	};

	enum BlendModes
	{
		MULTIPLY = 0,
		DARKEN = 1,
		LIGHTEN = 2,
		DIFFERENCES = 3,
		COLORBURN = 4,
		LINEARBURN = 5,
		SCREEN = 6,
		COLORDODGE = 7,
		LINEARDODGE = 8,
		SOFTLIGHT = 9,
		OVERLAY = 10
	};

	struct LayerVariables
	{
		GLfloat layerStrength[4] = {1.0f, 1.0f, 1.0f, 1.0f};
		GLuint useFirstLayerAsMask[4] = { 0,0,0,0 };
		GLuint blendModes[4] = { 0,0,0,0 };
		GLint numHeightmaps = 1;
	};

	GLuint ubo[1];

public:
    TerrainHeightPass();
    ~TerrainHeightPass();

	void Setup(Ptr<Render::TextureResource> baseMap);
	void Execute();

	void UpdateTexture(Render::TextureIndex index, const char* filename);
	//LayerVariables& GetLayerVariables() { return vars; }

	Ptr<Render::TextureResource> GetFinalHeightmap() { return mergedHeightmap; }
	Ptr<Render::TextureNode> GetHeightmaps() { return heightmaps; }

	LayerVariables layerVars;
private:

	Ptr<Render::TextureNode> heightmaps;
	Ptr<Render::TextureResource> mergedHeightmap;

	//Work group dimensions for the heightmap compute shader
	GLuint workGroupsX;
	GLuint workGroupsY;

	
	GLuint heightmapProgram;

};

} // namespace Terrain
