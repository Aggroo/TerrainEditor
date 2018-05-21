#pragma once
#include <string>

namespace Frames
{
class FramePass
{
public:
	FramePass();
	virtual ~FramePass();

	virtual void Setup();

	virtual void Execute();

protected:

	std::string name;

};
}