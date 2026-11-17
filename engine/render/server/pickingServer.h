#pragma once
#include "core/singleton.h"
#include "foundation/math/vec2.h"
#include "foundation/math/vec3.h"

#include <GL/glew.h>

namespace Math
{
	class vec4;
}

namespace Render
{

struct PickResult
{
	bool hit = false;
	Math::vec3 HitPosition;
	Math::vec2 UV;

};

class PickingServer
{
__DeclareSingleton(PickingServer)

public:

	PickResult PickTerrain(Math::vec2 viewMousePos);

private:

	bool ReadTerrainUV(int x, int glY, Math::vec2& outUV);
	Math::vec3 ReconstructWorldPosition(int x, int glY, float depth);

};
}