#include "config.h"
#include "meshresource.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Render
{
__ImplementClass(Render::MeshResources, 'MSHR', Core::RefCounted)
MeshResources::MeshResources() : vertexComponentMask(0), vertexSize(0), numVertices(0), vertexWidth(0), vertexPtr(nullptr), numIndices(0), indexSize(0), indexPtr(nullptr), renderable(false)
{
}

MeshResources::~MeshResources()
{
	glDeleteVertexArrays(1, &vao[0]);
	glDeleteBuffers(1, &vbo[0]);
	glDeleteBuffers(1, &ibo[0]);

}

void MeshResources::GenerateBuffers()
{
	glGenVertexArrays(1, &vao[0]);
	glBindVertexArray(vao[0]);

	glGenBuffers(1, &vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Render::Vertex)*mesh.Size(), &mesh[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2); 
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 14, NULL);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float32) * 14, (GLvoid*)(sizeof(float32) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 14, (GLvoid*)(sizeof(float32) * 5));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 14, (GLvoid*)(sizeof(float32) * 8));
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 14, (GLvoid*)(sizeof(float32) * 11));


	glGenBuffers(1, &ibo[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.Size(), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	this->numIndices = sizeof(GLuint) * indices.Size();
	renderable = true;
}

bool MeshResources::LoadMesh(const char* filename)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(filename, aiProcessPreset_TargetRealtime_Fast | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		T_CORE_ERROR("ERROR::ASSIMP::{0}", import.GetErrorString());
		return false;
	}

	Util::String path = filename;

	this->name = path.ExtractFileName();
	this->filePath = filename;

	aiMesh* firstMesh = scene->mMeshes[0];

	this->primitiveGroups.Reserve(scene->mNumMeshes);

	for (size_t i = 0; i < scene->mNumMeshes; i++)
		this->primitiveGroups.Append(PrimitiveGroup());

	if (firstMesh->HasPositions())
	{
		VertexComponent::SemanticName sem = VertexComponent::Position;
		VertexComponent::Format fmt = VertexComponent::Float3;
		this->vertexComponents.Append(VertexComponent(sem, 0, fmt));
		this->vertexWidth += 3;
		this->vertexComponentMask |= VertexComponentBits::Coord;
	}
	if (firstMesh->HasNormals())
	{
		VertexComponent::SemanticName sem = VertexComponent::Normal;
		VertexComponent::Format fmt = VertexComponent::Float3;
		this->vertexComponents.Append(VertexComponent(sem, 0, fmt));
		this->vertexWidth += 3;
		this->vertexComponentMask |= VertexComponentBits::Normal;
	}
	if (firstMesh->HasTextureCoords(0))
	{
		VertexComponent::SemanticName sem = VertexComponent::TexCoord1;
		VertexComponent::Format fmt = VertexComponent::Float2;
		this->vertexComponents.Append(VertexComponent(sem, 0, fmt));
		this->vertexWidth += 2;
		this->vertexComponentMask |= VertexComponentBits::Uv0;
	}
	if (firstMesh->HasTangentsAndBitangents())
	{
		VertexComponent::SemanticName sem = VertexComponent::Tangent;
		VertexComponent::Format fmt = VertexComponent::Float3;
		this->vertexComponents.Append(VertexComponent(sem, 0, fmt));
		this->vertexWidth += 3;
		this->vertexComponentMask |= VertexComponentBits::Tangent;

		sem = VertexComponent::Binormal;
		fmt = VertexComponent::Float3;
		this->vertexComponents.Append(VertexComponent(sem, 0, fmt));
		this->vertexWidth += 3;
		this->vertexComponentMask |= VertexComponentBits::Binormal;
	}
	if (firstMesh->HasVertexColors(0))
	{
		VertexComponent::SemanticName sem = VertexComponent::Color;
		VertexComponent::Format fmt = VertexComponent::Float3;
		this->vertexComponents.Append(VertexComponent(sem, 0, fmt));
		this->vertexWidth += 3;
		this->vertexComponentMask |= VertexComponentBits::Color;
	}
	//Sort to make sure that our components are in the correct order when we construct our vertexbuffer
	this->vertexComponents.Sort();

	//Count numverices and numindices
		//Set size and name for our primitive groups
	for (uint i = 0; i < scene->mNumMeshes; ++i)
	{
		//Add to total amount of vertices
		this->numVertices += scene->mMeshes[i]->mNumVertices;
		this->primitiveGroups[i].name = scene->mMeshes[i]->mName.C_Str();
		this->primitiveGroups[i].numIndices = 0;

		//count indices
		for (uint j = 0; j < scene->mMeshes[i]->mNumFaces; ++j)
		{
			this->numIndices += scene->mMeshes[i]->mFaces[j].mNumIndices;
			this->primitiveGroups[i].numIndices += scene->mMeshes[i]->mFaces[j].mNumIndices;
		}
	}

	this->vertexSize = this->numVertices * this->vertexWidth * sizeof(GLfloat);
	this->indexSize = this->numIndices * sizeof(int);

	this->meshPtr = new byte[this->vertexSize];
	this->indicesPtr = new byte[this->indexSize];

	//Point to the first element in our indexbuffer
	this->indexPtr = this->indicesPtr;

	this->vertexPtr = this->meshPtr;

	//Construct our vertex and indexbuffer
	{
		const byte triangleByteSize = 3 * sizeof(int);

		size_t sumNumVertices = 0;
		for (size_t i = 0; i < scene->mNumMeshes; ++i)
		{
			firstMesh = scene->mMeshes[i];

			for (size_t i = 0; i < firstMesh->mNumVertices; ++i)
			{
				for (uint j = 0; j < this->vertexComponents.Size(); j++)
				{
					size_t byteSize = this->vertexComponents[j].GetByteSize();

					switch (this->vertexComponents[j].GetSemanticName())
					{
					case VertexComponent::SemanticName::Position:
					{
						memcpy(this->vertexPtr, &firstMesh->mVertices[i], byteSize);
						break;
					}
					case VertexComponent::SemanticName::Normal:
					{
						memcpy(this->vertexPtr, &firstMesh->mNormals[i], byteSize);
						break;
					}
					case VertexComponent::SemanticName::TexCoord1:
					{
						memcpy(this->vertexPtr, &firstMesh->mTextureCoords[0][i], byteSize);
						break;
					}
					case VertexComponent::SemanticName::TexCoord2:
					{
						//Currently not supported
						memcpy(this->vertexPtr, &firstMesh->mTextureCoords[1][i], byteSize);
						break;
					}
					case VertexComponent::SemanticName::Tangent:
					{
						memcpy(this->vertexPtr, &firstMesh->mTangents[i], byteSize);
						break;
					}
					case VertexComponent::SemanticName::Binormal:
					{
						memcpy(this->vertexPtr, &firstMesh->mBitangents[i], byteSize);
						break;
					}
					case VertexComponent::SemanticName::Color:
					{
						memcpy(this->vertexPtr, &firstMesh->mColors[0][i], byteSize);
						break;
					}
					}

					this->vertexPtr = (byte*)vertexPtr + byteSize;
				}
			}

			for (size_t j = 0; j < firstMesh->mNumFaces; ++j)
			{
				if (firstMesh->mFaces[j].mNumIndices == 3)
				{
					//Triangles. Just append everything to the indexbuffer
					memcpy(this->indexPtr, firstMesh->mFaces[j].mIndices, triangleByteSize);

					//We need to compensate for appending them to the same indexbuffer. We do this by increasing every index with the sum of the number of vertices that are in the meshes before it
					*(GLuint*)indexPtr = *(GLuint*)this->indexPtr + (GLuint)sumNumVertices;
					*((GLuint*)indexPtr + 1) = *((GLuint*)indexPtr + 1) + (GLuint)sumNumVertices;
					*((GLuint*)indexPtr + 2) = *((GLuint*)indexPtr + 2) + (GLuint)sumNumVertices;

					this->indexPtr = (byte*)indexPtr + triangleByteSize;
				}
				else if (firstMesh->mFaces[j].mNumIndices == 2)
				{
					T_CORE_ERROR("ERROR: Meshloader does not support lines within meshes!");
					assert(false);
				}
				else if (firstMesh->mFaces[j].mNumIndices == 4)
				{
					T_CORE_ERROR("ERROR: Meshloader does not support quads within meshes!");
					assert(false);
				}
			}
			sumNumVertices += firstMesh->mNumVertices;
		}
	}

	this->primitiveGroups[0].indexOffset = 0;

	size_t offset = 0;

	for (size_t i = 0; i < scene->mNumMeshes; ++i)
	{
		//this->primitiveGroups[i].vertices = (size_t*)this->mesh + this->primitiveGroups[i - 1].vertexDataSize;
		this->primitiveGroups[i].indexOffset = offset;
		offset += this->primitiveGroups[i].numIndices * sizeof(int);
	}

	SetupVertexBuffer();
	SetupIndexBuffer();

	import.FreeScene();

	return true;
}

bool MeshResources::CreateMesh(const Util::Array<Render::Vertex>& vertexData, const size_t & numVertices, const Util::Array<GLuint>& indexData, const size_t & numIndices, int compBits)
{
	//Create a local variables and overwrite the member variable to stop adding too much to the member variables
	uint vertexComponentMask = 0;
	Util::Array<VertexComponent> vertexComponents;
	uint vertexWidth = 0;

	if (numVertices == 0 || numIndices == 0)
	{
		T_CORE_ERROR("ERROR: the number of vertice or indices are 0 in CreateMesh!");
		return false;
	}
	if (compBits & VertexComponentBits::Coord)
	{
		VertexComponent::SemanticName sem = VertexComponent::Position;
		VertexComponent::Format fmt = VertexComponent::Float3;
		vertexComponents.Append(VertexComponent(sem, 0, fmt));
		vertexWidth += 3;
		vertexComponentMask |= VertexComponentBits::Coord;
	}
	if (compBits & VertexComponentBits::Normal)
	{
		VertexComponent::SemanticName sem = VertexComponent::Normal;
		VertexComponent::Format fmt = VertexComponent::Float3;
		vertexComponents.Append(VertexComponent(sem, 0, fmt));
		vertexWidth += 3;
		vertexComponentMask |= VertexComponentBits::Normal;
	}
	if (compBits & VertexComponentBits::Uv0)
	{
		VertexComponent::SemanticName sem = VertexComponent::TexCoord1;
		VertexComponent::Format fmt = VertexComponent::Float2;
		vertexComponents.Append(VertexComponent(sem, 0, fmt));
		vertexWidth += 2;
		vertexComponentMask |= VertexComponentBits::Uv0;
	}
	if (compBits & (VertexComponentBits::Tangent | VertexComponentBits::Binormal))
	{
		VertexComponent::SemanticName sem = VertexComponent::Tangent;
		VertexComponent::Format fmt = VertexComponent::Float3;
		vertexComponents.Append(VertexComponent(sem, 0, fmt));
		vertexWidth += 3;
		vertexComponentMask |= VertexComponentBits::Tangent;

		sem = VertexComponent::Binormal;
		fmt = VertexComponent::Float3;
		vertexComponents.Append(VertexComponent(sem, 0, fmt));
		vertexWidth += 3;
		vertexComponentMask |= VertexComponentBits::Binormal;
	}
	if (compBits & VertexComponentBits::Color)
	{
		VertexComponent::SemanticName sem = VertexComponent::Color;
		VertexComponent::Format fmt = VertexComponent::Float3;
		vertexComponents.Append(VertexComponent(sem, 0, fmt));
		vertexWidth += 3;
		vertexComponentMask |= VertexComponentBits::Color;
	}
	//Sort to make sure that our components are in the correct order when we construct our vertexbuffer
	vertexComponents.Sort();

	this->vertexComponentMask = vertexComponentMask;
	this->vertexComponents = vertexComponents;
	this->vertexWidth = vertexWidth;

	this->numVertices = numVertices;
	this->numIndices = numIndices;

	this->vertexSize = this->numVertices * this->vertexWidth * sizeof(GLfloat);
	this->indexSize = this->numIndices * sizeof(GLuint);

	this->meshPtr = &vertexData[0];
	this->indicesPtr = &indexData[0];

	//Point to the first element in our indexbuffer
	this->indexPtr = this->indicesPtr;
	this->vertexPtr = this->meshPtr;

	//Create a local array and overwrite the member variable to overwrite the primitive groups instead of appending
	Util::Array<PrimitiveGroup> primitiveGroups;
	primitiveGroups.Append(PrimitiveGroup());
	primitiveGroups[0].name = "GeneratedMesh";
	primitiveGroups[0].indexOffset = 0;
	primitiveGroups[0].numIndices = this->indexSize;

	this->primitiveGroups = primitiveGroups;

	SetupVertexBuffer();
	SetupIndexBuffer();

	return true;
}

void MeshResources::createQuad()
{
	GLuint ibuff[6]
	{
		0, 3, 1,
		1, 3, 2
	};

	Vertex tempVertex1;
	tempVertex1.pos[0] = -1.0f; tempVertex1.pos[1] = -1.0f; tempVertex1.pos[2] = -1.0f;
	tempVertex1.uv[0] = 0.0f; tempVertex1.uv[1] = 0.0f;
	tempVertex1.norm[0] = 0.0f; tempVertex1.norm[1] = 0.0f; tempVertex1.norm[2] = 0.0f;
	mesh.Append(tempVertex1);

	Vertex tempVertex2;
	tempVertex2.pos[0] = -1.0f; tempVertex2.pos[1] = 1.0f; tempVertex2.pos[2] = -1.0f;
	tempVertex2.uv[0] = 0.0f; tempVertex2.uv[1] = 1.0f;
	tempVertex2.norm[0] = 0.0f; tempVertex2.norm[1] = 0.0f; tempVertex2.norm[2] = 0.0f;
	mesh.Append(tempVertex2);

	Vertex tempVertex3;
	tempVertex3.pos[0] = 1.0f; tempVertex3.pos[1] = 1.0f; tempVertex3.pos[2] = -1.0f;
	tempVertex3.uv[0] = 1.0f; tempVertex3.uv[1] = 1.0f;
	tempVertex3.norm[0] = 0.0f; tempVertex3.norm[1] = 0.0f; tempVertex3.norm[2] = 0.0f;
	mesh.Append(tempVertex3);

	Vertex tempVertex4;
	tempVertex4.pos[0] = 1.0f; tempVertex4.pos[1] = -1.0f; tempVertex4.pos[2] = -1.0f;
	tempVertex4.uv[0] = 1.0f; tempVertex4.uv[1] = 0.0f;
	tempVertex4.norm[0] = 0.0f; tempVertex4.norm[1] = 0.0f; tempVertex4.norm[2] = 0.0f;
	mesh.Append(tempVertex4);
		
	for (int i = 0; i < 6; i++)
	{
		indices.Append(ibuff[i]);
	}

}

void MeshResources::createQuad2()
{
	GLuint ibuff[6]
	{
		0, 1, 3,
		1, 2, 3
	};

	Vertex tempVertex1;
	tempVertex1.pos[0] = 0.0f; tempVertex1.pos[1] = 0.0f; tempVertex1.pos[2] = 0.0f;
	tempVertex1.uv[0] = 0.0f; tempVertex1.uv[1] = 0.0f;
	tempVertex1.norm[0] = 0.0f; tempVertex1.norm[1] = 0.0f; tempVertex1.norm[2] = 0.0f;
	mesh.Append(tempVertex1);

	Vertex tempVertex2;
	tempVertex2.pos[0] = 0.0f; tempVertex2.pos[1] = 1.0f; tempVertex2.pos[2] = 0.0f;
	tempVertex2.uv[0] = 0.0f; tempVertex2.uv[1] = 1.0f;
	tempVertex2.norm[0] = 0.0f; tempVertex2.norm[1] = 0.0f; tempVertex2.norm[2] = 0.0f;
	mesh.Append(tempVertex2);

	Vertex tempVertex3;
	tempVertex3.pos[0] = 1.0f; tempVertex3.pos[1] = 1.0f; tempVertex3.pos[2] = 0.0f;
	tempVertex3.uv[0] = 1.0f; tempVertex3.uv[1] = 1.0f;
	tempVertex3.norm[0] = 0.0f; tempVertex3.norm[1] = 0.0f; tempVertex3.norm[2] = 0.0f;
	mesh.Append(tempVertex3);

	Vertex tempVertex4;
	tempVertex4.pos[0] = 1.0f; tempVertex4.pos[1] = 0.0f; tempVertex4.pos[2] = 0.0f;
	tempVertex4.uv[0] = 1.0f; tempVertex4.uv[1] = 0.0f;
	tempVertex4.norm[0] = 0.0f; tempVertex4.norm[1] = 0.0f; tempVertex4.norm[2] = 0.0f;
	mesh.Append(tempVertex4);

	for (int i = 0; i < 6; i++)
	{
		indices.Append(ibuff[i]);
	}

}

void MeshResources::Draw()
{
	glBindVertexArray(vao[0]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
	glDrawElements(GL_TRIANGLES, this->numIndices, GL_UNSIGNED_INT, NULL);
		
}

void MeshResources::Draw(const unsigned int& group)
{
	GLsizei numind = (GLsizei)primitiveGroups[group].numIndices;
	uint offset = primitiveGroups[group].indexOffset;
	glDrawElements(GL_TRIANGLES, (GLsizei)primitiveGroups[group].numIndices, GL_UNSIGNED_INT, (void*)primitiveGroups[group].indexOffset);
}

void MeshResources::Bind()
{
	glBindVertexArray(vao[0]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
}

void MeshResources::Unbind()
{
	glBindVertexArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void MeshResources::SetupVertexBuffer()
{
	// setup new vertex buffer

	glGenVertexArrays(1, &vao[0]); // Create our Vertex Array Object  
	glBindVertexArray(vao[0]); // Bind our Vertex Array Object so we can use it  

	glGenBuffers(1, this->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, this->vertexSize, this->meshPtr, GL_STATIC_DRAW);


	uint attributeIndex = 0;
	size_t byteSize = 0;
	VertexComponent::Format format;
	GLenum type;
	GLboolean normalized;
	GLint components;

	GLbyte* offset = NULL;

	for (uint i = 0; i < this->vertexComponents.Size(); i++)
	{

		attributeIndex = (uint)this->vertexComponents[i].GetSemanticName();

		byteSize = this->vertexComponents[i].GetByteSize();
		format = this->vertexComponents[i].GetFormat();

		glEnableVertexAttribArray(attributeIndex);

		switch (format)
		{
		case VertexComponent::Float:	type = GL_FLOAT;		normalized = GL_FALSE;	components = 1;	break;
		case VertexComponent::Float2:	type = GL_FLOAT;		normalized = GL_FALSE;	components = 2;	break;
		case VertexComponent::Float3:	type = GL_FLOAT;		normalized = GL_FALSE;	components = 3;	break;
		case VertexComponent::Float4:	type = GL_FLOAT;		normalized = GL_FALSE;	components = 4;	break;
		case VertexComponent::UByte4:	type = GL_UNSIGNED_BYTE;	normalized = GL_FALSE;	components = 4;	break;
		case VertexComponent::Byte4:	type = GL_BYTE;			normalized = GL_FALSE;	components = 4;	break;
		case VertexComponent::Short2:	type = GL_SHORT;		normalized = GL_FALSE;	components = 2;	break;
		case VertexComponent::Short4:	type = GL_SHORT;		normalized = GL_FALSE;	components = 4;	break;
		case VertexComponent::UByte4N:	type = GL_UNSIGNED_BYTE;	normalized = GL_TRUE;	components = 4;	break;
		case VertexComponent::Byte4N:	type = GL_BYTE;			normalized = GL_TRUE;	components = 4;	break;
		case VertexComponent::Short2N:	type = GL_SHORT;		normalized = GL_TRUE;	components = 2;	break;
		case VertexComponent::Short4N:	type = GL_SHORT;		normalized = GL_TRUE;	components = 4;	break;
		}

		glVertexAttribPointer(attributeIndex, components, type, normalized, sizeof(GLfloat) * this->vertexWidth, (GLvoid*)offset);

		offset += byteSize;
	}
}

void MeshResources::SetupIndexBuffer()
{
	glGenBuffers(1, this->ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indexSize, this->indicesPtr, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

}