#pragma once
#include "GL/glew.h"
#include "foundation/math/vec2.h"
#include "foundation/math/vec3.h"
#include <map>
#include <vector>
#include "core/refcounted.h"
#include <string>
#include "vertexcomponent.h"

namespace Render
{	

struct PrimitiveGroup
{
	//name of this primitivegroup
	std::string name;

	//Pointer to this primitivegroups first element in the total buffer
	size_t indexOffset;

	//Number of indices in this group
	size_t numIndices;
};

struct Face
{
	std::vector<GLuint> verts;
};

struct Vertex
{
	Vertex()
	{
		
	}

	Vertex(Math::vec3 pos, Math::vec2 uv, Math::vec3 norm)
	{
		this->pos = pos;
		this->uv = uv;
		this->norm = norm;
	}

	Math::vec3 pos;
	Math::vec2 uv;
	Math::vec3 norm;
	Math::vec3 tangent;
	Math::vec3 binormal;
};

class MeshResources : public Core::RefCounted
{
__DeclareClass(MeshResources)
public:
	MeshResources();
	~MeshResources();

	///Bind and generate buffers, used by old rendering code and custom built meshes, like terrain
	void GenerateBuffers();

	///Imports a mesh using assimp
	bool LoadMesh(const char* filename);

	void createQuad();
	void createQuad2();

	///Draws all primitive groups
	void Draw();
	///Draw a single primitive group mesh
	void DrawPrimitiveGroup(const unsigned int& group);

	Util::Array<Render::Vertex>& GetMesh() { return mesh; };
	Util::Array<Render::Face>& GetMeshFaces() { return meshFaces; };
	Util::Array<GLuint>& GetIndices() { return indices; };

	Util::Array<Render::Vertex> mesh;
	Util::Array<GLuint> indices;
	Util::Array<Face> meshFaces;

	bool IsRenderable() { return renderable; }
	void SetRenderable(const bool& renderable) { this->renderable = renderable; }

private:

	void SetupVertexBuffer();
	void SetupIndexBuffer();

	// The filename for this mesh
	Util::String name;
	Util::String filePath;

	GLuint vbo[1];
	GLuint vao[1];
	GLuint ibo[1];

	///Contains all PrimitiveGroups vertices/indices
	void* meshPtr;
	void* indicesPtr;

	void* vertexPtr;
	void* indexPtr;

	uint numGroups;
	uint numVertices;
	uint vertexWidth;
	size_t numIndices;
	uint numEdges;

	uint vertexComponentMask;
	Util::Array<VertexComponent> vertexComponents;

	size_t groupDataSize;
	size_t vertexSize;
	size_t indexSize;

	Util::Array<PrimitiveGroup> primitiveGroups;
	
	bool renderable;
};

}
