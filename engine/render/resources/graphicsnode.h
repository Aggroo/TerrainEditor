#pragma once
#include "GL/glew.h"
#include "foundation/math/matrix4D.h"
#include "shaderobject.h"
#include "textureresource.h"
#include "meshresource.h"

#include <memory>
#include "../../../projects/terrain/code/terrain.h"

namespace CGMath
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

		void setTex(std::shared_ptr<TextureResource> newTex);
		std::shared_ptr<TextureResource> getTex();

		void setTerrain(std::shared_ptr<TerrainEditor::Terrain> newTerrain);
		std::shared_ptr<TerrainEditor::Terrain> getTerrain();
		
		void setTransMat(matrix4D newTransMat);
		matrix4D getTransMat();

		void SetViewMat(matrix4D newViewMat);
		matrix4D GetViewMat();

		void SetProjectionMat(matrix4D newProjectionMat);
		matrix4D GetProjectionMat();

		void draw();
		void drawSkinned();
		
	private:
		std::shared_ptr<ShaderObject> shader;
		std::shared_ptr<MeshResources> mesh;
		std::shared_ptr<TextureResource> tex;
		std::shared_ptr<TerrainEditor::Terrain> terr;
		matrix4D transMat;
		matrix4D viewMat;
		matrix4D projMat;
		
	};

}
