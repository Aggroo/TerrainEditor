#pragma once
#include "foundation/math/mat4.h"
#include "shaderobject.h"
#include "textureresource.h"
#include "meshresource.h"

#include <memory>
#include "../../../projects/terrain/code/terrain.h"
#include "texturenode.h"

namespace Math
{
	
	class GraphicsNode
	{
	public:
		GraphicsNode();
		~GraphicsNode();

		void setShaders(std::shared_ptr<ShaderObject> newShader);
		std::shared_ptr<ShaderObject> getShaders();

		void setMesh(std::shared_ptr<MeshResources> newMesh);
		std::shared_ptr<MeshResources> getMesh();

		void setTex(std::shared_ptr<TextureNode> newTex);
		std::shared_ptr<TextureNode> getTex();

		void setTerrain(std::shared_ptr<TerrainEditor::Terrain> newTerrain);
		std::shared_ptr<TerrainEditor::Terrain> getTerrain();
		
		void setTransMat(mat4 newTransMat);
		mat4 getTransMat();

		void SetViewMat(mat4 newViewMat);
		mat4 GetViewMat();

		void SetProjectionMat(mat4 newProjectionMat);
		mat4 GetProjectionMat();

		void draw();
		void drawSkinned();
		
	private:
		std::shared_ptr<ShaderObject> shader;
		std::shared_ptr<MeshResources> mesh;
		std::shared_ptr<TextureNode> tex;
		std::shared_ptr<TerrainEditor::Terrain> terr;
		mat4 transMat;
		mat4 viewMat;
		mat4 projMat;
		
	};

}
