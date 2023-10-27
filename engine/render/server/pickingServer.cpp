#include "config.h"
#include "pickingServer.h"
#include "foundation/math/mat4.h"
#include "foundation/math/vec4.h"
#include "render/camera/camera.h"

namespace Render
{

PickingServer::PickingServer()
{

}

Math::vec4 PickingServer::GetWorldSpacePosition(const int& mouseX, const int& mouseY, GLuint depthBufferFrameId)
{
    glBindFramebuffer(GL_FRAMEBUFFER, depthBufferFrameId);

    float depth;
    glReadPixels(mouseX, mouseY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

    float threshold = 0.99999f;
    if (depth < threshold)
    {
        // Convert the depth value to NDC
        float ndcDepth = 2.0f * depth - 1.0f;

        // Multiply the NDC depth by the inverse projection matrix
        Math::vec4 clipSpacePos(0.0f, 0.0f, ndcDepth, 1.0f);
        Math::mat4 inverseProjectionMatrix = Graphics::MainCamera::Instance()->GetInvProjection();
        Math::vec4 viewSpacePos = inverseProjectionMatrix * clipSpacePos;

        // Multiply the view-space depth by the inverse view matrix
        Math::mat4 inverseViewMatrix = Graphics::MainCamera::Instance()->GetInvView();
        Math::vec4 worldSpacePos = inverseViewMatrix * viewSpacePos;

        // Normalize the world-space position
        worldSpacePos = worldSpacePos / worldSpacePos.w();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        return worldSpacePos;
    }
    else
    {
        return Math::vec4(0.0f,0.0f,0.0f);
    }
    
}

}