#include "terrain.h"
#include "render/resources/stb_image.h"
#include <assert.h>
#include "config.h"

namespace TerrainEditor
{
Terrain::Terrain() : terrainWidth(0), terrainHeight(0), heightMap(nullptr)
{
}

Terrain::~Terrain()
{
	if (heightMap)
	{
		delete[] heightMap;
		heightMap = nullptr;
	}

	glDeleteVertexArrays(1, &vao[0]);
	glDeleteBuffers(1, &vbo[0]);
	glDeleteBuffers(1, &ibo[0]);
}

bool Terrain::CreateTerrain(const char* filename, float widthMultiplier, float heightMultiplier)
{
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
	mesh.reserve(vertexCount);

	// Create the index array.
	indices = new GLuint[indexCount];
	if (!indices)
	{
		return false;
	}

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
			mesh.push_back(TerrainVertex(CGMath::vector3D(heightMap[i].x, heightMap[i].y, heightMap[i].z), CGMath::vector2D(x*uDiv, -y*vDiv), CGMath::vector3D()));
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
			indices[index] = index3;
			index++;

			// Upper right.
			indices[index] = index4;
			index++;

			// Bottom left.
			indices[index] = index1;
			index++;

			// Bottom left.
			indices[index] = index1;
			index++;

			// Upper right.
			indices[index] = index4;
			index++;

			// Bottom right.
			indices[index] = index2;
			index++;

		}
	}

	GenerateNormals();

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

void Terrain::GenerateBuffer()
{
	glGenVertexArrays(1, &vao[0]);
	glBindVertexArray(vao[0]);

	glGenBuffers(1, &vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TerrainVertex)*mesh.size(), &mesh[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 8, NULL);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float32) * 8, (GLvoid*)(sizeof(float32) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 8, (GLvoid*)(sizeof(float32) * 5));

	glGenBuffers(1, &ibo[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indexCount, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Terrain::DrawTerrain()
{
	glBindVertexArray(vao[0]);

	/*glBindBuffer(GL_ARRAY_BUFFER, ibo[0]);
	glDrawArrays(GL_TRIANGLES, 0, mesh.size());*/
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, NULL);
}

float Terrain::GetHeightScale()
{
	return this->heightScale;
}

void Terrain::GenerateNormals()
{
	CGMath::vector3D p;
	int a, b, c;
	for (int i = 0; i < (indexCount - 3); i += 3)
	{
		a = indices[i];
		b = indices[i + 1];
		c = indices[i + 2];

		p = CGMath::vector3D::Cross(mesh[b].pos - mesh[a].pos, mesh[c].pos - mesh[a].pos);

		mesh[indices[i]].norm += p;
		mesh[indices[i + 1]].norm += p;
		mesh[indices[i + 2]].norm += p;
	}

	for (int i = 0; i < vertexCount; ++i)
	{
		mesh[i].norm = CGMath::vector3D::Normalize(mesh[i].norm);
	}
}
}
