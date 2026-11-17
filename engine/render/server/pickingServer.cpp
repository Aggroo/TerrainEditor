#include "config.h"
#include "pickingServer.h"
#include "foundation/math/mat4.h"
#include "foundation/math/vec4.h"
#include "render/camera/camera.h"
#include "render/render/renderer.h"
#include "render/server/frameserver.h"

namespace Render
{

PickingServer::PickingServer()
{

}

PickResult PickingServer::PickTerrain(Math::vec2 viewMousePos)
{
    GLuint depthBufferFrameId = Render::FrameServer::Instance()->GetDepthPass()->GetFrameBufferObject();

    const Render::Resolution& res = Render::Renderer::Instance()->GetRenderResolution();
    int x = (int)viewMousePos[0];
    int glY = (int)viewMousePos[1];

    if(x < 0 || x >= res.x || glY < 0 || glY >= res.y)
        return PickResult();
    Math::vec2 uv;
    if (ReadTerrainUV(x, glY, uv))
    {
        float depth = 1.0f;
        glBindFramebuffer(GL_READ_FRAMEBUFFER, depthBufferFrameId);
        glReadPixels(x, glY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

        Math::vec3 position = ReconstructWorldPosition(x, glY, depth);

        PickResult hitResult;
        hitResult.hit = true;
        hitResult.HitPosition = position;
        hitResult.UV = uv;

        return hitResult;
    }
    else
    {
        return PickResult();
    }
}

bool PickingServer::ReadTerrainUV(int x, int glY, Math::vec2& outUV)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, Render::FrameServer::Instance()->GetFlatGeometryLitPass()->GetFrameBufferObject());
    glReadBuffer(GL_COLOR_ATTACHMENT4);

    float rgb[3];
    glReadPixels(x, glY, 1, 1, GL_RGB, GL_FLOAT, rgb);

    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

    T_CORE_DEBUG("RGB: {0} {1} {2}", rgb[0], rgb[1], rgb[2]);

    if (rgb[2] > 0.5f)
    {
        outUV = Math::vec2(rgb[0], rgb[1]);
        return true;
    }

    return false;
}

Math::vec3 PickingServer::ReconstructWorldPosition(int x, int glY, float depth)
{
    const Render::Resolution& res = Render::Renderer::Instance()->GetRenderResolution();

    float ndcX = 2.0f * (x + 0.5f) / res.x - 1.0f;
    float ndcY = 2.0f * (glY + 0.5f) / res.y - 1.0f;
    float ndcDepth = 2.0f * depth - 1.0f;

    Math::vec4 clipSpacePos(ndcX, ndcY, ndcDepth, 1.0f);
    Math::mat4 inverseProjectionMatrix = Graphics::MainCamera::Instance()->GetInvProjection();
    Math::vec4 viewSpacePos = inverseProjectionMatrix * clipSpacePos;

    Math::mat4 inverseViewMatrix = Graphics::MainCamera::Instance()->GetInvView();
    Math::vec4 worldSpacePos = inverseViewMatrix * viewSpacePos;

    worldSpacePos = worldSpacePos / worldSpacePos.w();

    return worldSpacePos.ToVec3();
}

}