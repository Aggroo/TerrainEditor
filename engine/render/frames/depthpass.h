#pragma once
#include "drawpass.h"

namespace Render
{

class DepthPass : public DrawPass
{
__DeclareClass(DepthPass)
public:
	DepthPass();
	~DepthPass();

	void Setup();

	void Execute();

	void UpdateResolution();

	//Returns the handle of the position buffer
	GLuint& GetPositionBuffer() { return this->gPosition; }

	//Returns the handle of the normal buffer
	GLuint& GetNormalBuffer() { return this->gNormal; }

private:

	//Linearized depth buffer. Used for various screen-space effects.
	GLuint gPosition, gNormal, gAlbedoSpec;
};

}