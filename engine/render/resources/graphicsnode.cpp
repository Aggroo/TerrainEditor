#include "config.h"
#include "graphicsnode.h"
#include <iostream>
#include <fstream>
#include <string>
#include "render/camera/camera.h"
#include "render/render/renderer.h"


namespace Render
{
	__ImplementClass(Render::GraphicsNode, 'GFXN', Core::RefCounted)
	GraphicsNode::GraphicsNode()
	{

	}

	GraphicsNode::~GraphicsNode()
	{
		
	}


	void GraphicsNode::setShaders(Ptr<Render::ShaderObject> newShader)
	{
		this->shader = newShader;
	}

	Ptr<Render::ShaderObject> GraphicsNode::getShaders()
	{
		return this->shader;
	}


	void GraphicsNode::setMesh(std::shared_ptr<MeshResources> newMesh)
	{
		this->mesh = newMesh;
	}

	std::shared_ptr<MeshResources> GraphicsNode::getMesh()
	{
		return this->mesh;
	}

	void GraphicsNode::setTex(Ptr<Render::TextureNode> newTex)
	{
		this->tex = newTex;
	}

	Ptr<Render::TextureNode> GraphicsNode::getTex()
	{
		return this->tex;
	}

	void GraphicsNode::setTransMat(Math::mat4 newTransMat)
	{
		this->transMat = newTransMat;
	}

	Math::mat4 GraphicsNode::getTransMat()
	{
		return this->transMat;
	}

	void GraphicsNode::SetViewMat(Math::mat4 newViewMat)
	{
		this->viewMat = newViewMat;
	}

	Math::mat4 GraphicsNode::GetViewMat()
	{
		return this->viewMat;
	}

	void GraphicsNode::SetProjectionMat(Math::mat4 newProjectionMat)
	{
		this->projMat = newProjectionMat;
	}

	Math::mat4 GraphicsNode::GetProjectionMat()
	{
		return this->projMat;
	}


	void GraphicsNode::draw()
	{
		this->shader->BindProgram();
		this->tex->BindTextures();
		this->shader->setupMatrix4fv("transMatrix", this->transMat);
		this->shader->setupMatrix3fv("normalMat", Math::mat3::Transpose(Math::mat3::fromMatrix4D(this->transMat).invert()));

		if (this->mesh->mesh.Size() != 0)
			this->mesh->drawMesh();
	}

	void GraphicsNode::drawSkinned()
	{
		this->shader->BindProgram();
		this->tex->BindTextures();
		this->shader->setupMatrix4fv("transMatrix", this->transMat);
		this->mesh->drawSkinnedMesh();
	}

}
