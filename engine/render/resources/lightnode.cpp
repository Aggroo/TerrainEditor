#include "config.h"
#include "lightnode.h"
#include <iostream>
#include <fstream>
#include <string>


namespace Math
{
	LightNode::LightNode()
	{

	}

	LightNode::~LightNode()
	{
		
	}
	

	void LightNode::setPos(vec4 pos)
	{
		this->pos = pos;
	}

	void LightNode::setPos(GLfloat x, GLfloat y, GLfloat z)
	{
		vec4 vec(x,y,z);

		this->pos = vec;
	}

	vec4 LightNode::getPos()
	{
		return this->pos;
	}

	void LightNode::setColour(GLfloat r, GLfloat g, GLfloat b)
	{
		vec3 cV;
		cV.x() = r;
		cV.y() = g;
		cV.z() = b;

		this->colour = cV;
	}

	vec3 LightNode::getColour()
	{
		return this->colour;
	}

	void LightNode::setIntensity(GLfloat a, GLfloat b, GLfloat c)
	{
		vec3 cV(a,b,c);

		this->intensity = cV;
	}

	vec3 LightNode::getIntensity()
	{
		return this->intensity;
	}

	void LightNode::setShaders(std::shared_ptr<ShaderObject> newShader)
	{
		this->shaders = newShader;
	}

	std::shared_ptr<ShaderObject> LightNode::getShaders()
	{
		return this->shaders;
	}

	void LightNode::apply()
	{
		shaders->setupVector3f("lightPosition", pos.x(), pos.y(), pos.z());
		shaders->setupVector3f("u_lightAmbientIntensity", colour.x()*intensity.x(), colour.y()*intensity.y(), colour.z()*intensity.z());
		shaders->setupVector3f("u_lightDiffuseIntensity", 0.8f*intensity.x(), 0.8f*intensity.y(), 0.8f*intensity.z());
		shaders->setupVector3f("u_lightSpecularIntensity", 0.8f*intensity.x(), 0.8f*intensity.y(), 0.5f*intensity.z());

	}

}