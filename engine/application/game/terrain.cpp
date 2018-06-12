#include "config.h"
#include "terrain.h"
#include "render/resources/stb_image.h"
#include <assert.h>
#include <iostream>

namespace TerrainEditor
{
Terrain::Terrain() : terrainWidth(0), terrainHeight(0), heightMap(nullptr)
{
	mesh = std::make_shared<Math::MeshResources>();
}

Terrain::~Terrain()
{
	if (heightMap)
	{
		delete[] heightMap;
		heightMap = nullptr;
	}
}

void Terrain::Activate()
{
	Entity::Activate();
}

void Terrain::Deactivate()
{
	Entity::Deactivate();
}

void Terrain::Update()
{
	mesh->drawMesh();
}

bool Terrain::CreateTerrain(const char* filename, float widthMultiplier, float heightMultiplier)
{
	mesh->mesh.clear();
	int n;
	unsigned char *image = stbi_load(filename, &terrainWidth, &terrainHeight, &n, 0);

	if (image == nullptr)
	{
		std::cerr << "Could not read file " << filename << ". File does not exist." << std::endl;
		return false;
	}

	this->heightScale = heightMultiplier;
	heightMap = new HeightmapValues[terrainWidth*terrainHeight];

	float heightVal;
	int k = 0;
	int index;
	for (int y = 0; y < terrainHeight; ++y)
	{
		for (int x = 0; x < terrainWidth; ++x)
		{
			heightVal = static_cast<float>(image[k]);
			heightVal += static_cast<float>(image[k + 1]);
			heightVal += static_cast<float>(image[k + 2]);
			heightVal /= 3.f;
			heightVal /= 255.f;

			index = (terrainHeight * y) + x;

			heightMap[index].x = static_cast<float>(x*widthMultiplier);
			heightMap[index].y = static_cast<float>(heightVal*heightMultiplier);
			heightMap[index].z = static_cast<float>(y*widthMultiplier);

			k += 3;
		}
	}

	vertexCount = ((terrainWidth-1) * (terrainHeight-1));
	indexCount = vertexCount*6;

	// Create the vertex array.
	mesh->mesh.reserve(vertexCount);

	// Create the index array.
	mesh->indices.assign(indexCount, 0);

	SmoothenTerrain();

	// Initialize the index to the vertex buffer.
	index = 0;
	int index1, index2, index3, index4;
	float uDiv = 1.f / terrainWidth;
	float vDiv = 1.f / terrainHeight;

	int i;
	for (int y = 0; y < (terrainHeight); ++y)
	{
		for (int x = 0; x < (terrainWidth); ++x)
		{
			i = (terrainHeight * y) + x;
			mesh->mesh.push_back(Math::Vertex(Math::vec3(heightMap[i].x, heightMap[i].y, heightMap[i].z), Math::vec2(x*uDiv, -y*vDiv), Math::vec3()));
		}
	}

	// Load the vertex and index array with the terrain data.
	for (int y = 0; y < (terrainHeight-1); ++y)
	{
		for (int x = 0; x < (terrainWidth-1); ++x)
		{
			index1 = (terrainHeight * y) + x;          // Bottom left.
			index2 = (terrainHeight * y) + (x + 1);      // Bottom right.
			index3 = (terrainHeight * (y + 1)) + x;      // Upper left.
			index4 = (terrainHeight * (y + 1)) + (x + 1);  // Upper right.

			// Upper left.
			mesh->indices[index] = index3;
			index++;

			// Upper right.
			mesh->indices[index] = index4;
			index++;

			// Bottom left.
			mesh->indices[index] = index1;
			index++;

			// Bottom left.
			mesh->indices[index] = index1;
			index++;

			// Upper right.
			mesh->indices[index] = index4;
			index++;

			// Bottom right.
			mesh->indices[index] = index2;
			index++;

		}
	}

	GenerateNormals();

	mesh->genBuffer();

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
	Math::vec3 p;
	int a, b, c;
	for (int i = 0; i < (indexCount - 3); i += 3)
	{
		a = mesh->indices[i];
		b = mesh->indices[i + 1];
		c = mesh->indices[i + 2];

		p = Math::vec3::Cross(mesh->mesh[b].pos - mesh->mesh[a].pos, mesh->mesh[c].pos - mesh->mesh[a].pos);

		mesh->mesh[mesh->indices[i]].norm += p;
		mesh->mesh[mesh->indices[i + 1]].norm += p;
		mesh->mesh[mesh->indices[i + 2]].norm += p;
	}

	for (int i = 0; i < vertexCount; ++i)
	{
		mesh->mesh[i].norm = Math::vec3::Normalize(mesh->mesh[i].norm);
	}
}
}
