#ifndef HALFEDGEMESH_H
#define HALFEDGEMESH_H
#include "foundation/math/vec3.h"
#include "foundation/math/vec2.h"
#include "meshresource.h"
#include <map>

using namespace Math;

namespace HalfEdges
{
struct Face;
struct Vertex;

struct HalfEdge
{
	HalfEdge* oppositeEdge;
	HalfEdge* nextEdge;
	HalfEdges::Vertex* vert;
	HalfEdges::Face* face;
};

struct Face
{
	HalfEdges::HalfEdge* faceEdge;
};

struct Vertex
{
	vec3 pos;
	vec2 uv;
	vec3 norm;
	int idx;
	HalfEdge* edge;
};

struct heMesh
{
	Util::Array<GLuint> indices;
	Util::Array<Math::Vertex> mesh;
};

class HalfEdgeMesh
{
public:
	HalfEdgeMesh();
	HalfEdgeMesh(MeshResources* mesh);
	~HalfEdgeMesh();
	
	void ConstructHalfEdgeMesh();
	MeshResources* HalfEdgeMeshToMesh();
	
	void Subdivide(int iterations);
	
private:
	
	MeshResources* mesh;
	MeshResources tempMesh;
	HalfEdges::heMesh halfEdgeToMesh;
	
	Util::Array<HalfEdges::Vertex*> vertices;
	Util::Array<HalfEdges::Face*> faces;
	Util::Array<HalfEdges::HalfEdge*> halfedges;
	std::map< std::pair<unsigned int, unsigned int>, HalfEdge* > edges;
	std::map<unsigned int, int> heMeshIndices;
};
}

#endif // HALFEDGEMESH_H
