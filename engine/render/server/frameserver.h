#pragma once
#include "core/singleton.h"
#include "core/refcounted.h"
#include "render/frames/framepass.h"
#include "render/frames/depthpass.h"
#include "render/frames/flatgeometrylitpass.h"

namespace Render
{
class FrameServer
{
__DeclareSingleton(FrameServer)

public:
	void SetupFramePasses();

	void UpdateResolutions() const;

	Ptr<FramePass> GetFramePass(const Util::String& name);

	bool HasPassNamed(const Util::String& name);

	Ptr<DepthPass> GetDepthPass();
	Ptr<FramePass> GetLightCullingPass();
	Ptr<FlatGeometryLitPass> GetFlatGeometryLitPass();

private:

	friend class Renderer;

	//Contains all Framepasses.
	//Key must be unique to each Pass. the key is the pass name
	Util::Dictionary<Util::String, Ptr<FramePass>> framePassByName;
	Util::Array<Ptr<FramePass>> framePasses;

	/// Early depth testing
	Ptr<DepthPass> Depth;

	/// Used for lightculling as part of tiled forward rendering.
	Ptr<FramePass> lightCullingPass;

	/// For lit objects
	Ptr<FlatGeometryLitPass> FlatGeometryLit;
};
}
