#pragma once
#include "GL/glew.h"
#include "application/game/entity.h"
#include <memory>
#include "render/resources/meshresource.h"
#include "render/resources/texturenode.h"
#include "render/resources/shaderobject.h"
#include "imgui.h"

namespace TerrainEditor
{

struct HeightmapValues
{
	float x, y, z;

	HeightmapValues() : x(0), y(0), z(0) {}
};

class Terrain : public Game::Entity
{
	__DeclareClass(Terrain);
public:
	Terrain();
	~Terrain();

	///Registers this entity to the Entity server
	void Activate();
	void Deactivate();

	virtual void Update();

	///Generates the terrain from a Heightmap
	bool CreateTerrain(const char* filename, float widthMultiplier, float heightMultiplier, ImVec2* points);

	HeightmapValues Average(int x, int y);

	float GetSteepness(int x, int y);

	float GetHeightScale();

	Ptr<Math::MeshResources> GetMesh() { return mesh; }
	Ptr<Math::ShaderObject> GetShader() { return shader; }
	Ptr<Render::TextureNode> GetTextures() { return textures; }

private:
	void SmoothenTerrain();
	bool inBounds(int x, int y);
	void GenerateNormals();
	float GetHeight(int x, int y) const;

	int terrainWidth;
	int terrainHeight;

	float heightScale;
	float highestPoint;

	HeightmapValues* heightMap;

///Rendering

	GLuint indexCount;
	GLuint vertexCount;

	Ptr<Render::TextureNode> textures;
	Ptr<Math::MeshResources> mesh;
	Ptr<Math::ShaderObject> shader;

};
}


