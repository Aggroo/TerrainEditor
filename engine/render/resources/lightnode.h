#pragma once
#include "GL/glew.h"
#include "foundation/math/mat4.h"
#include "shaderobject.h"

#include <memory>

namespace Render
{
class LightNode : public Core::RefCounted
{
__DeclareClass(LightNode)
public:
	LightNode();
	~LightNode();

	void setPos(Math::vec4 pos);
	void setPos(GLfloat x, GLfloat y, GLfloat z);
	Math::vec4 getPos();

	void setColour(GLfloat r, GLfloat g, GLfloat b);
	Math::vec3 getColour();

	void setIntensity(GLfloat a, GLfloat b, GLfloat c);
	Math::vec3 getIntensity();

	void setShaders(Ptr<Render::ShaderObject> newShader);
	Ptr<Render::ShaderObject> getShaders();

	void apply();
	
private:
	Ptr<Render::ShaderObject> shaders;

	Math::vec4 pos;
	Math::vec3 colour;
	Math::vec3 intensity;

	
};

}