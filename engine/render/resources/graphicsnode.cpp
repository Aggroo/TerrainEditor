#include "config.h"
#include "graphicsnode.h"
#include <iostream>
#include <fstream>
#include <string>
#include "render/camera/camera.h"


namespace Math
{
	GraphicsNode::GraphicsNode()
	{

	}

	GraphicsNode::~GraphicsNode()
	{
		
	}


	void GraphicsNode::setShaders(std::shared_ptr<ShaderObject> newShader)
	{
		this->shader = newShader;
	}

	std::shared_ptr<ShaderObject> GraphicsNode::getShaders()
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

	void GraphicsNode::setTex(std::shared_ptr<TextureNode> newTex)
	{
		this->tex = newTex;
	}

	std::shared_ptr<TextureNode> GraphicsNode::getTex()
	{
		return this->tex;
	}

	void GraphicsNode::setTerrain(std::shared_ptr<TerrainEditor::Terrain> newTerrain)
	{
		this->terr = newTerrain;
	}

	std::shared_ptr<TerrainEditor::Terrain> GraphicsNode::getTerrain()
	{
		return this->terr;
	}

	void GraphicsNode::setTransMat(mat4 newTransMat)
	{
		this->transMat = newTransMat;
	}

	Math::mat4 GraphicsNode::getTransMat()
	{
		return this->transMat;
	}

	void GraphicsNode::SetViewMat(mat4 newViewMat)
	{
		this->viewMat = newViewMat;
	}

	mat4 GraphicsNode::GetViewMat()
	{
		return this->viewMat;
	}

	void GraphicsNode::SetProjectionMat(mat4 newProjectionMat)
	{
		this->projMat = newProjectionMat;
	}

	mat4 GraphicsNode::GetProjectionMat()
	{
		return this->projMat;
	}


	void GraphicsNode::draw()
	{
		this->shader->useProgram();
		this->tex->BindTextures();
		this->shader->setupMatrix4fv("transMatrix", this->transMat);
		this->shader->setupMatrix4fv("Vmat", Graphics::MainCamera::Instance()->GetView());
		this->shader->setupMatrix4fv("Pmat", Graphics::MainCamera::Instance()->GetPerspective());
		this->shader->setupMatrix3fv("normalMat", mat3::Transpose(mat3::fromMatrix4D(this->transMat).invert()));
		if(this->terr != nullptr)
			this->terr->DrawTerrain();
		if (this->mesh != nullptr)
			this->mesh->drawMesh();
	}

	void GraphicsNode::drawSkinned()
	{
		this->shader->useProgram();
		this->tex->BindTextures();
		this->shader->setupMatrix4fv("transMatrix", this->transMat);
		this->mesh->drawSkinnedMesh();
	}

}
