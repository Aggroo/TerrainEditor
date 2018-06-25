#pragma once
#include "core/refcounted.h"
#include "drawpass.h"

namespace Render
{
class FlatGeometryLitPass : public DrawPass
{
__DeclareClass(FlatGeometryLitPass)
public:
	FlatGeometryLitPass();
	~FlatGeometryLitPass();

	void Setup();

	void Execute();

	void UpdateResolution();

private:


};
}
