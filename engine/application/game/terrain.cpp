#include "config.h"
#include "terrain.h"
#include "stb_image.h"
#include <assert.h>
#include <iostream>
#include "foundation/util/curve.hpp"
#include "render/camera/camera.h"
#include "render/render/renderer.h"
#include "render/server/resourceserver.h"
#include "application/basegamefeatures/entitymanager.h"
#include "render/resources/model.h"

namespace TerrainEditor
{
__ImplementClass(TerrainEditor::Terrain, 'TETY', Game::EntityBase);

Terrain::Terrain() : terrainWidth(0), terrainHeight(0), heightMap(nullptr), generate(false), node(nullptr)
{
	mesh = Render::Model::Create();
	this->surface = Render::Surface::Create();

	glGenBuffers(1, this->ubo);
}

Terrain::~Terrain()
{
	if (heightMap)
	{
		delete[] heightMap;
		heightMap = nullptr;
	}
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Terrain::Activate()
{	

	this->surface = Render::ResourceServer::Instance()->LoadSurface("resources/surfaces/terrain.sur");

	Render::UBOInfo* bufferInfo = new Render::UBOInfo{ sizeof(TerrainShaderVariables), &tsVar };

	this->surface->SetUniformBuffer(bufferInfo);

	EntityBase::Activate();
}

void Terrain::Deactivate()
{
	EntityBase::Deactivate();
}

void Terrain::Update()
{
	this->mesh->transform = this->transform;
}

void Terrain::OnUI()
{
	
}

bool Terrain::CreateTerrain(const char* filename, int size, float widthMultiplier, float heightMultiplier, ImVec2* points)
{

	this->vertices.Reset();
	this->indices.Reset();
	this->mesh->GetMesh() = Render::MeshResources::Create();
	
	tsVar.heightScale = heightMultiplier;

	this->terrainWidth = size;
	this->terrainHeight = size;
	this->sizeModifier = widthMultiplier;
	
	generate = true;

	this->vertexCount = this->terrainWidth * this->terrainHeight;
	this->indexCount = vertexCount * 6;

	// Create the vertex array.
	this->vertices.Reserve(vertexCount);

	// Create the index array.
	this->indices.Fill(0, indexCount, 0);

	// Initialize the index to the vertex buffer.
	int index = 0;
	int index1, index2, index3, index4;
	float uDiv = 1.f / this->terrainWidth;
	float vDiv = 1.f / this->terrainHeight;

	int i;
	for (int y = 0; y < (this->terrainHeight); ++y)
	{
		for (int x = 0; x < (this->terrainWidth); ++x)
		{
			i = (this->terrainHeight * y) + x;
			this->vertices.Append(Render::Vertex(Math::vec3(x*this->sizeModifier, 0.0f, y*this->sizeModifier), Math::vec2(x*uDiv, -y * vDiv), Math::vec3()));
		}
	}

	// Load the vertex and index array with the terrain data.
	for (int y = 0; y < (this->terrainHeight - 1); ++y)
	{
		for (int x = 0; x < (this->terrainWidth - 1); ++x)
		{
			index1 = (this->terrainHeight * y) + x;          // Bottom left.
			index2 = (this->terrainHeight * y) + (x + 1);      // Bottom right.
			index3 = (this->terrainHeight * (y + 1)) + x;      // Upper left.
			index4 = (this->terrainHeight * (y + 1)) + (x + 1);  // Upper right.

			// Upper left.
			this->indices[index] = index3;
			index++;

			// Upper right.
			this->indices[index] = index4;
			index++;

			// Bottom left.
			this->indices[index] = index1;
			index++;

			// Bottom left.
			this->indices[index] = index1;
			index++;

			// Upper right.
			this->indices[index] = index4;
			index++;

			// Bottom right.
			this->indices[index] = index2;
			index++;

		}
	}

	this->mesh->GetMesh()->CreateMesh(this->vertices, vertexCount, this->indices, this->indexCount, 
		Render::VertexComponentBits::Coord | Render::VertexComponentBits::Uv0 | Render::VertexComponentBits::Normal | Render::VertexComponentBits::Tangent);

	if (this->node == NULL)
	{
		this->node = new Render::ModelNode;
		this->node->modelInstance = mesh;
		this->node->primitiveGroup = 0;
		this->node->surface = this->surface.get();

		this->mesh->AddModelNode(node);
		this->surface->AddModelNode(node);
	}

	//GenerateNormals();

	//this->mesh->GenerateBuffers();

	return true;
}


void Terrain::SmoothenTerrain()
{
	HeightmapValues* dest = new HeightmapValues[terrainWidth*terrainHeight];

	for (uint i = 0; i < terrainHeight; ++i)
	{
		for (uint j = 0; j < terrainWidth; ++j)
		{
			dest[i*terrainHeight + j] = Average(j, i);
		}
	}

	for (uint i = 0; i < (terrainWidth*terrainHeight); ++i)
	{
		heightMap[i] = dest[i];
	}

	delete[] dest;
}

HeightmapValues Terrain::Average(int x, int y)
{
	HeightmapValues avg;
	float num = 0;
	for (int m = y - 1; m <= y + 1; ++m)
	{
		for (int n = x - 1; n <= x + 1; ++n)
		{
			if (inBounds(m, n))
			{
				avg.x += heightMap[m*terrainHeight + n].x;
				avg.y += heightMap[m*terrainHeight + n].y;
				avg.z += heightMap[m*terrainHeight + n].z;
				num += 1.0f;
			}
		}
	}
	avg.x /= num;
	avg.y /= num;
	avg.z /= num;
	return avg;
}

float Terrain::GetSteepness(int x, int y)
{
	float slopeX = GetHeight(x <  terrainWidth - 1 ? x + 1 : x, y) - GetHeight(x > 0 ? x - 1 : x, y);
	float slopeZ = GetHeight(x, y <  terrainHeight - 1 ? y + 1 : y) - GetHeight(x, y > 0 ? y - 1 : y);

	if (x == 0 || x == terrainWidth - 1)
		slopeX *= 2;
	if (y == 0 || y == terrainHeight - 1)
		slopeZ *= 2;

	// Heightmap width = heightmap height
	Math::vec3 normal = Math::vec3(-slopeX * (terrainWidth - 1) / terrainWidth, (terrainWidth - 1) / highestPoint, slopeZ * (terrainHeight - 1) / terrainHeight);
	normal = Math::vec3::Normalize(normal);

	return acos(Math::vec3::Dot(normal, Math::vec3(0.f, 1.f, 0.f)));
}

bool Terrain::inBounds(int x, int y)
{
	return ((x >= 0 && x < this->terrainWidth) && (y >= 0 && y < this->terrainHeight));
}


float Terrain::GetHeightScale()
{
	return this->heightScale;
}

void Terrain::GenerateNormals()
{
	//Math::vec3 p;
	//int a, b, c;
	//for (int i = 0; i < (indexCount - 3); i += 3)
	//{
	//	a = mesh->indices[i];
	//	b = mesh->indices[i + 1];
	//	c = mesh->indices[i + 2];
	//
	//	p = Math::vec3::Cross(mesh->mesh[b].pos - mesh->mesh[a].pos, mesh->mesh[c].pos - mesh->mesh[a].pos);
	//
	//	mesh->mesh[mesh->indices[i]].norm += p;
	//	mesh->mesh[mesh->indices[i + 1]].norm += p;
	//	mesh->mesh[mesh->indices[i + 2]].norm += p;
	//}
	//
	//for (int i = 0; i < vertexCount; ++i)
	//{
	//	mesh->mesh[i].norm = Math::vec3::Normalize(mesh->mesh[i].norm);
	//}

	for (int i = 0; i < vertexCount; i += 3)
	{
		// Edges of the triangle : position delta
		Math::vec3 deltaPos1 = this->vertices[i+1].pos - this->vertices[i].pos;
		Math::vec3 deltaPos2 = this->vertices[i + 2].pos - this->vertices[i].pos;

		// UV delta
		Math::vec2 deltaUV1 = this->vertices[i + 1].uv - this->vertices[i].uv;
		Math::vec2 deltaUV2 = this->vertices[i + 2].uv - this->vertices[i].uv;

		float r = 1.0f / (deltaUV1[0] * deltaUV2[1] - deltaUV1[1] * deltaUV2[0]);
		Math::vec3 tangent = (deltaPos1 * deltaUV2[1] - deltaPos2 * deltaUV1[1])*r;
		Math::vec3 bitangent = (deltaPos2 * deltaUV1[0] - deltaPos1 * deltaUV2[0])*r;

		this->vertices[i + 0].tangent = tangent;
		this->vertices[i + 1].tangent = tangent;
		this->vertices[i + 2].tangent = tangent;

		this->vertices[i + 0].binormal = bitangent;
		this->vertices[i + 1].binormal = bitangent;
		this->vertices[i + 2].binormal = bitangent;
	}
}

float Terrain::GetHeight(int x, int y) const
{
	return this->vertices[y*terrainWidth + x].pos.y();
}

void Terrain::BindShaderVariables()
{
	glBindBuffer(GL_UNIFORM_BUFFER, this->ubo[0]);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, this->ubo[0]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(TerrainShaderVariables), &tsVar, GL_STATIC_DRAW);

}
}
