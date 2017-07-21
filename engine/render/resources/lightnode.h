#pragma once
#include "GL/glew.h"
#include "foundation/math/matrix4D.h"
#include "shaderobject.h"

#include <memory>

namespace CGMath
{
	class LightNode
	{
	public:
		LightNode();
		~LightNode();

		void setPos(vector4D pos);
		void setPos(GLfloat x, GLfloat y, GLfloat z);
		vector4D getPos();

		void setColour(GLfloat r, GLfloat g, GLfloat b);
		vector3D getColour();

		void setIntensity(GLfloat a, GLfloat b, GLfloat c);
		vector3D getIntensity();

		void setShaders(std::shared_ptr<ShaderObject> newShader);
		std::shared_ptr<ShaderObject> getShaders();

		void apply();
		
	private:
		std::shared_ptr<ShaderObject> shaders;

		vector4D pos;
		vector3D colour;
		vector3D intensity;

		
	};

}