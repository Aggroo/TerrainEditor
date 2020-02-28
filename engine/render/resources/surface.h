#pragma once
//------------------------------------------------------------------------------
/**
    Class to handle the surface of a mesh

    (C) 2020 Anton Grönberg
*/
//------------------------------------------------------------------------------
#include "core/refcounted.h"
#include "material.h"

namespace Render
{

class Surface : public Core::RefCounted
{
__DeclareClass(Surface);

public:
    Surface();
    ~Surface();

private:
    
    Render::TextureNode textures;

};
} // namespace Render
