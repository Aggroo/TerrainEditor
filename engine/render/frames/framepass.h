#pragma once
#include <string>
#include "core/refcounted.h"

namespace Render
{
class FramePass : public Core::RefCounted
{
__DeclareClass(FramePass)
public:
	FramePass();
	virtual ~FramePass();

	//Setups the Pass
	virtual void Setup();

	//Execute the Pass
	virtual void Execute();

protected:
	friend class Renderer;
	friend class FrameServer;
	Util::String name;

};
}
