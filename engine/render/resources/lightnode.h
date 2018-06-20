#pragma once
#include "GL/glew.h"
#include "foundation/math/mat4.h"
#include "shaderobject.h"

#include <memory>

namespace Math
{
	class LightNode
	{
	public:
		LightNode();
		~LightNode();

		void setPos(vec4 pos);
		void setPos(GLfloat x, GLfloat y, GLfloat z);
		vec4 getPos();

		void setColour(GLfloat r, GLfloat g, GLfloat b);
		vec3 getColour();

		void setIntensity(GLfloat a, GLfloat b, GLfloat c);
		vec3 getIntensity();

		void setShaders(Ptr<ShaderObject> newShader);
		Ptr<ShaderObject> getShaders();

		void apply();
		
	private:
		Ptr<ShaderObject> shaders;

		vec4 pos;
		vec3 colour;
		vec3 intensity;

		
	};

}