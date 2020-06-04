#pragma once
#include "foundation/math/mat4.h"
#include "shaderobject.h"
#include "textureresource.h"
#include "meshresource.h"

#include <memory>
#include "texturenode.h"

namespace Render
{
	
	class GraphicsNode : public Core::RefCounted
	{
		__DeclareClass(GraphicsNode)
	public:
		GraphicsNode();
		~GraphicsNode();

		void setShaders(Ptr<Render::ShaderObject> newShader);
		Ptr<Render::ShaderObject> getShaders();

		void setMesh(std::shared_ptr<MeshResources> newMesh);
		std::shared_ptr<MeshResources> getMesh();

		void setTex(Ptr<Render::TextureNode> newTex);
		Ptr<Render::TextureNode> getTex();
		
		void setTransMat(Math::mat4 newTransMat);
		Math::mat4 getTransMat();

		void SetViewMat(Math::mat4 newViewMat);
		Math::mat4 GetViewMat();

		void SetProjectionMat(Math::mat4 newProjectionMat);
		Math::mat4 GetProjectionMat();

		void draw();
		void drawSkinned();
		
	private:
		Ptr<Render::ShaderObject> shader;
		std::shared_ptr<MeshResources> mesh;
		Ptr<Render::TextureNode> tex;

		Math::mat4 transMat;
		Math::mat4 viewMat;
		Math::mat4 projMat;
		
	};

}
