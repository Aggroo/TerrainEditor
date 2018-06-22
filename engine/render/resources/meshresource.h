#pragma once
#include "GL/glew.h"
#include "foundation/math/vec2.h"
#include "foundation/math/vec3.h"
#include <map>
#include <vector>
#include "core/refcounted.h"


namespace Math
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

		vec3 pos;
		vec2 uv;
		vec3 norm;
		vec3 tangent;
		vec3 binormal;
	};
	
	class MeshResources : public Core::RefCounted
	{
		__DeclareClass(MeshResources)
	public:
		MeshResources();
		MeshResources(Util::Array<Math::Vertex>, Util::Array<GLuint>);
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
		Util::Array<Math::Vertex> GetMesh();
		Util::Array<Math::Face>& GetMeshFaces();
		Util::Array<GLuint> getIndices();

		//void SetTangents(bool t) { tangents = t; }

		Util::Array<Math::Vertex> mesh;
		Util::Array<GLuint> indices;
		Util::Array<Face> meshFaces;

		//Util::Array<Math::vec3> tangentList;
		//Util::Array<Math::vec3> bitangentList;

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
		
		
	};

}
