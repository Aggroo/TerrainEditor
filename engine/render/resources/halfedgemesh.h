#ifndef HALFEDGEMESH_H
#define HALFEDGEMESH_H
#include "foundation/math/vector3D.h"
#include "foundation/math/vector2D.h"
#include "meshresource.h"
#include <map>

using namespace CGMath;

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
	vector3D pos;
	vector2D uv;
	vector3D norm;
	int idx;
	HalfEdge* edge;
};

struct heMesh
{
	std::vector<GLuint> indices;
	std::vector<CGMath::Vertex> mesh;
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
	
	std::vector<HalfEdges::Vertex*> vertices;
	std::vector<HalfEdges::Face*> faces;
	std::vector<HalfEdges::HalfEdge*> halfedges;
	std::map< std::pair<unsigned int, unsigned int>, HalfEdge* > edges;
	std::map<unsigned int, int> heMeshIndices;
};
}

#endif // HALFEDGEMESH_H
