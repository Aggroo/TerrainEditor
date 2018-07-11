#include "config.h"
#include "terrain.h"
#include "render/resources/stb_image.h"
#include <assert.h>
#include <iostream>
#include "foundation/util/curve.hpp"
#include "render/camera/camera.h"
#include "render/render/renderer.h"
#include "render/server/shaderserver.h"
#include "application/basegamefeatures/entitymanager.h"
#include "render/render/skybox.h"

namespace TerrainEditor
{
__ImplementClass(TerrainEditor::Terrain, 'TETY', Game::Entity);

Terrain::Terrain() : terrainWidth(0), terrainHeight(0), heightMap(nullptr)
{
	mesh = Render::MeshResources::Create();
	shader = Render::ShaderObject::Create();
	textures = Render::TextureNode::Create();
	p[0] = transform[3]; 
	p[1] = transform[7];
	p[2] = transform[11];
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
	GLuint vert = Render::ShaderServer::Instance()->LoadVertexShader("resources/shaders/terrainPBR.vert");
	GLuint frag = Render::ShaderServer::Instance()->LoadFragmentShader("resources/shaders/terrainPBR.frag");

	shader->AddShader(vert);
	shader->AddShader(frag);
	shader->LinkShaders();

	Render::ShaderServer::Instance()->AddShaderObject("Terrain-Phong", shader);

	shader->BindProgram();

	textures->AddTexture(Render::TextureIndex::albedo0, "resources/textures/terrain_textures/mossy-ground/mixedmoss-albedo2.png");
	textures->AddTexture(Render::TextureIndex::albedo1, "resources/textures/terrain_textures/slate-cliff/slatecliffrock-albedo.png");
	textures->AddTexture(Render::TextureIndex::albedo2, "resources/textures/terrain_textures/marble-speckled/marble-speckled-albedo.png");

	textures->AddTexture(Render::TextureIndex::normal0, "resources/textures/terrain_textures/mossy-ground/mixedmoss-normal2.png");
	textures->AddTexture(Render::TextureIndex::normal1, "resources/textures/terrain_textures/slate-cliff/slatecliffrock_Normal.jpg");
	textures->AddTexture(Render::TextureIndex::normal2, "resources/textures/terrain_textures/marble-speckled/marble-speckled-normal.png");

	textures->AddTexture(Render::TextureIndex::specular0, "resources/textures/terrain_textures/mossy-ground/mixedmoss-metalness.png");
	textures->AddTexture(Render::TextureIndex::specular1, "resources/textures/terrain_textures/slate-cliff/slatecliffrock_Metallic.png");
	textures->AddTexture(Render::TextureIndex::specular2, "resources/textures/terrain_textures/marble-speckled/marble-speckled-metalness.png");

	textures->AddTexture(Render::TextureIndex::roughness0, "resources/textures/terrain_textures/mossy-ground/mixedmoss-roughness.png");
	textures->AddTexture(Render::TextureIndex::roughness1, "resources/textures/terrain_textures/slate-cliff/slatecliffrock_Roughness2.png");
	textures->AddTexture(Render::TextureIndex::roughness2, "resources/textures/terrain_textures/marble-speckled/marble-speckled-roughness.png");
			
	textures->AddTexture(Render::TextureIndex::splat, "resources/textures/heightmaps/splat2.jpg");

	//textures.AddTexture("resources/textures/pathway.jpg");


	shader->setupUniformInt("textures[0]", (GLuint) Render::TextureIndex::albedo0);
	shader->setupUniformInt("textures[1]", (GLuint) Render::TextureIndex::albedo1);
	shader->setupUniformInt("textures[2]", (GLuint) Render::TextureIndex::albedo2);

	shader->setupUniformInt("normals[0]", (GLuint) Render::TextureIndex::normal0);
	shader->setupUniformInt("normals[1]", (GLuint) Render::TextureIndex::normal1);
	shader->setupUniformInt("normals[2]", (GLuint) Render::TextureIndex::normal2);

	shader->setupUniformInt("specular[0]", (GLuint)Render::TextureIndex::specular0);
	shader->setupUniformInt("specular[1]", (GLuint)Render::TextureIndex::specular1);
	shader->setupUniformInt("specular[2]", (GLuint)Render::TextureIndex::specular2);

	shader->setupUniformInt("metallic[0]", (GLuint)Render::TextureIndex::roughness0);
	shader->setupUniformInt("metallic[1]", (GLuint)Render::TextureIndex::roughness1);
	shader->setupUniformInt("metallic[2]", (GLuint)Render::TextureIndex::roughness2);

	shader->setupUniformInt("splat", (GLuint) Render::TextureIndex::splat);

	shader->setupUniformFloat("tex0UvMultiplier", 0.1f);
	shader->setupUniformFloat("tex1UvMultiplier", 0.1f);
	shader->setupUniformFloat("tex2UvMultiplier", 0.1f);

	Entity::Activate();
}

void Terrain::Deactivate()
{
	Entity::Deactivate();
}

void Terrain::Update()
{
	const Ptr<Render::Skybox> sky = BaseGameFeature::EntityManager::Instance()->GetEntityByID(GetID() + 1).downcast<Render::Skybox>();
	this->shader->BindProgram();
	this->textures->BindTextures();
	this->shader->setupMatrix4fv("Model", this->transform);
	this->shader->setupVector3f("cameraPosition", Graphics::MainCamera::Instance()->GetCameraPosition());
	this->shader->setupUniformInt("prefilterMap", sky->GetCubemap());

	if(mesh->mesh.Size() != 0)
		mesh->drawMesh();
}

void Terrain::OnUI()
{
	if (ImGui::CollapsingHeader("Terrain Entity"))
	{
		
		if(ImGui::DragFloat3("Position", p))
		{
			transform.SetPosition(Math::vec4(p[0], p[1], p[2], 1.0f));
		}
	}
	
}

bool Terrain::CreateTerrain(const char* filename, float widthMultiplier, float heightMultiplier, ImVec2* points)
{
	mesh->mesh.Reset();
	mesh->indices.Reset();
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
			heightMap[index].y = static_cast<float>(ImGui::CurveValue(heightVal, 10, points)*heightMultiplier);
			heightMap[index].z = static_cast<float>(y*widthMultiplier);

			k += 3;
		}
	}

	vertexCount = ((terrainWidth) * (terrainHeight));
	indexCount = vertexCount*6;

	// Create the vertex array.
	mesh->mesh.Reserve(vertexCount);

	// Create the index array.
	mesh->indices.Fill(0, indexCount, 0);

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
			mesh->mesh.Append(Render::Vertex(Math::vec3(heightMap[i].x, heightMap[i].y, heightMap[i].z), Math::vec2(x*uDiv, -y*vDiv), Math::vec3()));
		}
	}

	highestPoint = FLT_MIN;

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

			if (mesh->mesh[y*terrainWidth+x].pos.y() > highestPoint)
			{
				highestPoint = mesh->mesh[y*terrainWidth + x].pos.y();
			}

		}
	}

	GenerateNormals();
	GetSteepness(100, 200);
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

	for (int i = 0; i < vertexCount; i += 3)
	{
		// Edges of the triangle : position delta
		Math::vec3 deltaPos1 = mesh->mesh[i+1].pos - mesh->mesh[i].pos;
		Math::vec3 deltaPos2 = mesh->mesh[i + 2].pos - mesh->mesh[i].pos;

		// UV delta
		Math::vec2 deltaUV1 = mesh->mesh[i + 1].uv - mesh->mesh[i].uv;
		Math::vec2 deltaUV2 = mesh->mesh[i + 2].uv - mesh->mesh[i].uv;

		float r = 1.0f / (deltaUV1[0] * deltaUV2[1] - deltaUV1[1] * deltaUV2[0]);
		Math::vec3 tangent = (deltaPos1 * deltaUV2[1] - deltaPos2 * deltaUV1[1])*r;
		Math::vec3 bitangent = (deltaPos2 * deltaUV1[0] - deltaPos1 * deltaUV2[0])*r;

		mesh->mesh[i + 0].tangent = tangent;
		mesh->mesh[i + 1].tangent = tangent;
		mesh->mesh[i + 2].tangent = tangent;

		mesh->mesh[i + 0].binormal = bitangent;
		mesh->mesh[i + 1].binormal = bitangent;
		mesh->mesh[i + 2].binormal = bitangent;
	}
}

float Terrain::GetHeight(int x, int y) const
{
	return mesh->mesh[y*terrainWidth + x].pos.y();
}
}
