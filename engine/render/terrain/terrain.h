#pragma once
#include "GL/glew.h"
#include "application/game/entitybase.h"
#include <memory>
#include "render/resources/surface.h"
#include "imgui.h"
#include "foundation/util/jobsystem.h"
#include "render/resources/meshresource.h"
#include "render/resources/Model.h"


namespace Terrain
{

struct HeightmapValues
{
	float x, y, z;

	HeightmapValues() : x(0), y(0), z(0) {}
};

class Terrain : public Game::EntityBase
{
	__DeclareClass(Terrain);
private:
	struct TerrainShaderVariables
	{
		float heightScale = 1.0f;
		float texUv0Multiplier = 0.1f;
		float texUv1Multiplier = 0.1f;
		float texUv2Multiplier = 0.1f;
		float slopeAngle = 0.8f;
		float heightFalloff = 10.0f;
		float height = 10.0f;
		float slopeAngle2 = 0.8f;
		float heightFalloff2 = 40.0f;
		float height2 = 60.0f;
		float hardness1 = 10.0f;
		float hardness2 = 10.0f;
		float hardness3 = 4.0f;
		int numHeightmaps = 1;
	};

	///uniform buffer object
	GLuint ubo[1];

public:
	Terrain();
	~Terrain();

	///Registers this entity to the Entity server
	void Activate();
	void Deactivate();

	virtual void Update();

	virtual void OnUI();

	///Generates the terrain from a Heightmap
	bool CreateTerrain(const char* filename, int size, float widthMultiplier, float heightMultiplier);

	float GetHeightScale();

	Ptr<Render::Model> GetModel() { return mesh; }
	Ptr<Render::Surface> GetSurface() { return surface; }

	///Shader Settings
	TerrainShaderVariables tsVar;

private:
	void SmoothenTerrain();
	bool inBounds(int x, int y);
	void GenerateNormals();
	float GetHeight(int x, int y) const;
	void BindShaderVariables();

	int terrainWidth;
	int terrainHeight;

	float sizeModifier;

	float heightScale;
	float highestPoint;

	HeightmapValues* heightMap;

///Rendering

	GLuint indexCount;
	GLuint vertexCount;

	Ptr<Render::Surface> surface;
	Ptr<Render::Model> mesh;

	Util::Array<Render::Vertex> vertices;
	Util::Array<GLuint> indices;

	Render::ModelNode* node;
};
}


