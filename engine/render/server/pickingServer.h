#pragma once
#include "core/singleton.h"

#include <GL/glew.h>

namespace Math
{
	class vec4;
}

namespace Render
{
class PickingServer
{
__DeclareSingleton(PickingServer)

public:

	Math::vec4 GetWorldSpacePosition(const int& mouseX, const int& mouseY, GLuint depthBufferFrameId);

private:

};
}