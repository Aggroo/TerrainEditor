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

	//Returns the handle of the linear depth buffer
	GLuint GetLinearDepthBuffer() { return this->linearDepthBuffer; }

private:

	//Linearized depth buffer. Used for various screen-space effects.
	GLuint linearDepthBuffer;
};

}