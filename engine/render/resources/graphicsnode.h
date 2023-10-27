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

		void SetShaders(Ptr<Render::ShaderObject> newShader);
		Ptr<Render::ShaderObject> GetShaders();

		void SetMesh(Ptr<MeshResources> newMesh);
		Ptr<MeshResources> GetMesh();

		void SetTex(Ptr<Render::TextureNode> newTex);
		Ptr<Render::TextureNode> GetTex();
		
		void SetTransMat(Math::mat4 newTransMat);
		Math::mat4 GetTransMat();

		void SetViewMat(Math::mat4 newViewMat);
		Math::mat4 GetViewMat();

		void SetProjectionMat(Math::mat4 newProjectionMat);
		Math::mat4 GetProjectionMat();

		void Draw();
		void DrawSkinned();
		
	private:
		Ptr<Render::ShaderObject> shader;
		Ptr<MeshResources> mesh;
		Ptr<Render::TextureNode> tex;

		Math::mat4 transMat;
		Math::mat4 viewMat;
		Math::mat4 projMat;
		
	};

}
