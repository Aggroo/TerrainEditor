#pragma once
#include <string>
#include "framepass.h"

namespace Frames
{
class DrawPass : public FramePass
{
public:
	DrawPass();
	~DrawPass();

	virtual void Setup();

	virtual void Execute();

private:


};
}
