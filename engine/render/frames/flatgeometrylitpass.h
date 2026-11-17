#pragma once
#include "core/refcounted.h"
#include "drawpass.h"

namespace Render
{
class FlatGeometryLitPass final : public DrawPass
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

	GLuint GetUVBuffer() { return this->uvBuffer; }

private:
	GLuint normalBuffer;
	GLuint specularBuffer;
	GLuint roughnessBuffer;
	GLuint uvBuffer;

	static constexpr GLenum drawBuffers[5] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };

	Ptr<Render::Skybox> skybox;

};
}
