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

	GLuint GetNormalBuffer() { return this->normalBuffer; }

	GLuint GetSpecularBuffer() { return this->specularBuffer; }

	GLuint GetRoughnessBuffer() { return this->roughnessBuffer; }

private:
	GLuint normalBuffer;
	GLuint specularBuffer;
	GLuint roughnessBuffer;


};
}
