#pragma once
//------------------------------------------------------------------------------
/**
    Height pass for terrain. Runs a compute shader to blend multiple heightmaps into one

    (C) 2020 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/refcounted.h"

namespace Terrain
{
class TerrainHeightPass : public Core::RefCounted
{
    __DeclareClass(TerrainHeightPass);
public:
    TerrainHeightPass();
    ~TerrainHeightPass();

private:

};

} // namespace Terrain
