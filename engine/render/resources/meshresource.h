#pragma once
#include "GL/glew.h"
#include "foundation/math/vec2.h"
#include "foundation/math/vec3.h"
#include <map>
#include <vector>
#include "core/refcounted.h"


namespace Render
{	
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
	MeshResources(Util::Array<Render::Vertex>, Util::Array<GLuint>);
	~MeshResources();

	void genBuffer();
	void genSkinnedBuffer(size_t vertexSize, uchar* vertexPtr, size_t indexSize, uchar* indexPtr, GLuint size);
	bool loadObj(const char* filename);
	void createQuad();
	void createQuad2();
	void drawMesh();
	void drawSkinnedMesh();

	//Used to concatenating three ints to a string
	std::string FaceKey(int pos,int uv,int norm) const;
	Util::Array<Render::Vertex> GetMesh();
	Util::Array<Render::Face>& GetMeshFaces();
	Util::Array<GLuint> getIndices();

	//void SetTangents(bool t) { tangents = t; }

	Util::Array<Render::Vertex> mesh;
	Util::Array<GLuint> indices;
	Util::Array<Face> meshFaces;

	//Util::Array<Math::vec3> tangentList;
	//Util::Array<Math::vec3> bitangentList;

	bool IsRenderable() { return renderable; }
	void SetRenderable(const bool& renderable) { this->renderable = renderable; }

private:

	//bool tangents;

	GLuint vbo[1];
	GLuint vao[1];
	GLuint ibo[1];
	GLuint tSize;
	GLuint iSize;

	size_t vertexSize;
	uchar* vertexPtr;

	size_t indexSize;
	uchar* indexPtr;

	//Used to keep the loader from creating double vertices
	std::map<std::string, GLuint> vertexMap;
	
	bool renderable;
};

}
