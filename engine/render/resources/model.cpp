#include "config.h"
#include "model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Render
{
__ImplementClass(Render::Model, 'MODL', Core::RefCounted);
Model::Model()
{
}

Model::~Model()
{
}

void Model::LoadModel(Util::String path)
{
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(path.AsCharPtr(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		printf("ERROR::ASSIMP::%s", import.GetErrorString());
		return;
	}
	directory = path.ExtractDirName();

	ProcessNode(scene->mRootNode, scene);

}

void Model::Draw()
{
	for (unsigned int i = 0; i < meshes.Size(); i++)
	{
		if (meshes[i]->IsRenderable())
			meshes[i]->drawMesh();
	}	
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	//process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.Append(ProcessMesh(mesh, scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Ptr<MeshResources> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	Util::Array<Vertex> vertices;
	Util::Array<GLuint> indices;

	// Walk through each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		Math::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
		// positions
		vector[0] = mesh->mVertices[i].x;
		vector[1] = mesh->mVertices[i].y;
		vector[2] = mesh->mVertices[i].z;
		vertex.pos = vector;
		// normals
		vector[0] = mesh->mNormals[i].x;
		vector[1] = mesh->mNormals[i].y;
		vector[2] = mesh->mNormals[i].z;
		vertex.norm = vector;
		// texture coordinates
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			Math::vec2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec[0] = mesh->mTextureCoords[0][i].x;
			vec[1] = mesh->mTextureCoords[0][i].y;
			vertex.uv = vec;
		}
		else
			vertex.uv = Math::vec2(0.0f, 0.0f);

		if(mesh->HasTangentsAndBitangents())
		{
			// tangent
			vector[0] = mesh->mTangents[i].x;
			vector[1] = mesh->mTangents[i].y;
			vector[2] = mesh->mTangents[i].z;
			vertex.tangent = vector;
			// bitangent
			vector[0] = mesh->mBitangents[i].x;
			vector[1] = mesh->mBitangents[i].y;
			vector[2] = mesh->mBitangents[i].z;
			vertex.binormal = vector;
		}
		
		vertices.Append(vertex);
	}

	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.Append(face.mIndices[j]);
	}
	Ptr<MeshResources> finalMesh = MeshResources::Create();
	finalMesh->indices = indices;
	finalMesh->mesh = vertices;
	finalMesh->genBuffer();
	return finalMesh;
}
}
