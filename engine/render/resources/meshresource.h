#pragma once
#include "GL/glew.h"
#include "foundation/math/vector2D.h"
#include "foundation/math/vector3D.h"
#include "foundation/math/vector4D.h"
#include <map>
#include <vector>


namespace CGMath
{
	struct Vertex;
	
	struct Face
	{
		std::vector<GLuint> verts;
	};
	
	struct Vertex
	{
		vector3D pos;
		vector2D uv;
		vector3D norm;
	};
	
	class MeshResources
	{
	public:
		MeshResources();
		MeshResources(std::vector<CGMath::Vertex>, std::vector<GLuint>);
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
		std::vector<CGMath::Vertex> GetMesh();
		std::vector<CGMath::Face>& GetMeshFaces();
		std::vector<GLuint> getIndices();
		
		std::vector<CGMath::Vertex> mesh;
		std::vector<GLuint> indices;
		std::vector<Face> meshFaces;

	private:

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
		
		
	};

}