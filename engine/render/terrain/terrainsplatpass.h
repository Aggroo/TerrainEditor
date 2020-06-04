#pragma once
//------------------------------------------------------------------------------
/**
    Terrain Splat Pass. Runs a compute shader to generate a splatmap

    (C) 2020 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/refcounted.h"

namespace Terrain
{
class TerrainSplatPass : public Core::RefCounted
{
    __DeclareClass(TerrainSplatPass);
public:
    TerrainSplatPass();
    ~TerrainSplatPass();

private:

    
};
} // namespace Terrain
