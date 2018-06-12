#pragma once
#include "GL/glew.h"
#include "foundation/math/vec3.h"
#include "foundation/math/vec2.h"
#include <vector>
#include "application/game/entity.h"
#include <memory>
#include "render/resources/meshresource.h"

namespace TerrainEditor
{

struct HeightmapValues
{
	float x, y, z;

	HeightmapValues() : x(0), y(0), z(0) {}
};

class Terrain : public Game::Entity
{
public:
	Terrain();
	~Terrain();

	///Registers this entity to the physics server
	void Activate();
	void Deactivate();

	void Update() override;

	///Generates the terrain from a Heightmap
	bool CreateTerrain(const char* filename, float widthMultiplier, float heightMultiplier);

	HeightmapValues Average(int x, int y);

	float GetHeightScale();

	std::shared_ptr<Math::MeshResources> GetMesh() { return mesh; }

private:
	void SmoothenTerrain();
	bool inBounds(int x, int y);
	void GenerateNormals();

	int terrainWidth;
	int terrainHeight;

	float heightScale;

	HeightmapValues* heightMap;

///Rendering

	GLuint indexCount;
	GLuint vertexCount;

	std::shared_ptr<Math::MeshResources> mesh;

};
}


