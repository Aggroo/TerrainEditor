#pragma once
#include "drawpass.h"

namespace Frames
{

class DepthPass : public DrawPass
{
public:
	DepthPass();
	~DepthPass();

	void Setup();

	void Execute();

};

}