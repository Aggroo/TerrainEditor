#pragma once
#include "drawpass.h"

namespace Render
{

class IBLPass : public DrawPass
{
__DeclareClass(IBLPass)
public:
	IBLPass();
	~IBLPass();

	void Setup();


	void Execute();

	void UpdateResolution();

	void UpdateIBL(Util::String newTexturePath);

	Ptr<TextureResource> GetEnvironmentMap() const { return sp; }
	Ptr<TextureResource> GetIrradianceMap() const { return ir; }
	Ptr<TextureResource> GetBRDFMap() const { return brdf; }

	const Util::String& GetHDRTexturePath() const { return hdrTexPath; }

	static inline int numMipMapLevels(int width, int height)
	{
		int levels = 1;
		while ((width | height) >> levels) {
			++levels;
		}
		return levels;
	}

private:
	
	void ComputeShaderIBL();

	void FrameBufferIBL();
	
	Ptr<TextureResource> ir, brdf, sp;

	Util::String hdrTexPath;
};

}
