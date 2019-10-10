#pragma once
#include "GL/glew.h"
#include "application/game/entitybase.h"
#include <memory>
#include "render/resources/meshresource.h"
#include "render/resources/texturenode.h"
#include "render/resources/shaderobject.h"
#include "imgui.h"
#include "render/render/skybox.h"

namespace TerrainEditor
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
		float texUv0Multiplier = 0.1f;
		float texUv1Multiplier = 0.1f;
		float texUv2Multiplier = 0.1f;
		float slopeAngle = 0.8f;
		float heightFalloff = 10.0f;
		float height = 5.0f;
		float slopeAngle2 = 0.8f;
		float heightFalloff2 = 40.0f;
		float height2 = 60.0f;
		float hardness1 = 0.2f;
		float hardness2 = 0.1f;
		float hardness3 = 4.0f;
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
	bool CreateTerrain(const char* filename, float widthMultiplier, float heightMultiplier, ImVec2* points);

	HeightmapValues Average(int x, int y);

	float GetSteepness(int x, int y);

	float GetHeightScale();

	Ptr<Render::MeshResources> GetMesh() { return mesh; }
	Ptr<Render::ShaderObject> GetShader() { return shader; }
	Ptr<Render::TextureNode> GetTextures() { return textures; }

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

	float heightScale;
	float highestPoint;

	HeightmapValues* heightMap;

///Rendering

	GLuint indexCount;
	GLuint vertexCount;

	Ptr<Render::TextureNode> textures;
	Ptr<Render::MeshResources> mesh;
	Ptr<Render::ShaderObject> shader;

};
}


