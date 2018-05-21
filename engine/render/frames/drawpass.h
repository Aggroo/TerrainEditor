#pragma once
#include <string>

namespace Frames
{
class DrawPass
{
public:
	DrawPass();
	~DrawPass();

	virtual void Setup();

	virtual void Execute();

private:


};
}