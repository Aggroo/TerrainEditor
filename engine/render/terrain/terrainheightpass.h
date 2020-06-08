#pragma once
//------------------------------------------------------------------------------
/**
    Height pass for terrain. Runs a compute shader to blend multiple heightmaps into one

    (C) 2020 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/refcounted.h"

namespace Render
{
class TextureNode;
class TextureResource;
}

namespace Terrain
{

struct LayerVariables
{
	int numHeightmaps;
	bool useFirstLayerAsMask[5];
	float layerStrength[5];
};

class TerrainHeightPass : public Core::RefCounted
{
    __DeclareClass(TerrainHeightPass);
public:
    TerrainHeightPass();
    ~TerrainHeightPass();

	void Setup();
	void Execute();

	void UpdateVariables(const LayerVariables& updatedVars);

private:

	Ptr<Render::TextureNode> heightmaps;
	Ptr<Render::TextureResource> mergedHeightmap;

	LayerVariables vars;

};

} // namespace Terrain
