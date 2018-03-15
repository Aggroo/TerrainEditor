#pragma once
#include "GL/glew.h"
#include "foundation/math/vector3D.h"
#include "foundation/math/vector2D.h"
#include <vector>

namespace TerrainEditor
{
struct TerrainVertex
{
	TerrainVertex(CGMath::vector3D pos, CGMath::vector2D uv, CGMath::vector3D norm)
	{
		this->pos = pos;
		this->uv = uv;
		this->norm = norm;
	}

	CGMath::vector3D pos;
	CGMath::vector2D uv;
	CGMath::vector3D norm;

};

struct HeightmapValues
{
	float x, y, z;

	HeightmapValues() : x(0), y(0), z(0) {}
};

class Terrain
{
public:
	Terrain();
	~Terrain();

	///Generates the terrain from a Heightmap
	bool CreateTerrain(const char* filename, float widthMultiplier, float heightMultiplier);

	HeightmapValues Average(int x, int y);

	void GenerateBuffer();
	void DrawTerrain();

	float GetHeightScale();

private:
	void SmoothenTerrain();
	bool inBounds(int x, int y);
	void GenerateNormals();

	int terrainWidth;
	int terrainHeight;

	float heightScale;

	HeightmapValues* heightMap;

///Rendering values
	GLuint vbo[1];
	GLuint vao[1];
	GLuint ibo[1];

	GLuint indexCount;
	GLuint vertexCount;

	std::vector<TerrainVertex> mesh;
	GLuint* indices;

};
}


