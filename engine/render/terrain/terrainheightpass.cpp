//------------------------------------------------------------------------------
//  terrainheightpass.cpp
//  (C) 2020 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "terrainheightpass.h"
#include "render/resources/texturenode.h"

namespace Terrain
{
__ImplementClass(Terrain::TerrainHeightPass, 'TEHP', Core::RefCounted);


TerrainHeightPass::TerrainHeightPass() : vars{ 1, {1,1,1,1,1}, {1.0f,1.0f,1.0f,1.0f,1.0f} }
{
}

void TerrainHeightPass::UpdateVariables(const LayerVariables& updatedVars)
{
	this->vars = updatedVars;
}

} // namespace Terrain
