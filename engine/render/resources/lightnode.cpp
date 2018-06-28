#include "config.h"
#include "lightnode.h"
#include <iostream>
#include <fstream>
#include <string>


namespace Render
{
__ImplementClass(Render::LightNode, 'RELN', Core::RefCounted)
LightNode::LightNode()
{

}

LightNode::~LightNode()
{
	
}


void LightNode::setPos(Math::vec4 pos)
{
	this->pos = pos;
}

void LightNode::setPos(GLfloat x, GLfloat y, GLfloat z)
{
	Math::vec4 vec(x,y,z);

	this->pos = vec;
}

Math::vec4 LightNode::getPos()
{
	return this->pos;
}

void LightNode::setColour(GLfloat r, GLfloat g, GLfloat b)
{
	Math::vec3 cV;
	cV.x() = r;
	cV.y() = g;
	cV.z() = b;

	this->colour = cV;
}

Math::vec3 LightNode::getColour()
{
	return this->colour;
}

void LightNode::setIntensity(GLfloat a, GLfloat b, GLfloat c)
{
	Math::vec3 cV(a,b,c);

	this->intensity = cV;
}

Math::vec3 LightNode::getIntensity()
{
	return this->intensity;
}

void LightNode::setShaders(Ptr<Render::ShaderObject> newShader)
{
	this->shaders = newShader;
}

Ptr<Render::ShaderObject> LightNode::getShaders()
{
	return this->shaders;
}

void LightNode::apply()
{
	shaders->BindProgram();
	shaders->setupVector3f("lightPosition", pos.x(), pos.y(), pos.z());
	shaders->setupVector3f("u_lightAmbientIntensity", colour.x()*intensity.x(), colour.y()*intensity.y(), colour.z()*intensity.z());
	shaders->setupVector3f("u_lightDiffuseIntensity", 0.8f*intensity.x(), 0.8f*intensity.y(), 0.8f*intensity.z());
	shaders->setupVector3f("u_lightSpecularIntensity", 0.8f*intensity.x(), 0.8f*intensity.y(), 0.5f*intensity.z());

}

}