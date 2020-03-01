#include "config.h"
#include "model.h"
#include <assimp/pbrmaterial.h>
#include "render/server/frameserver.h"

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
	const aiScene *scene = import.ReadFile(path.AsCharPtr(), aiProcessPreset_TargetRealtime_Fast | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		printf("ERROR::ASSIMP::%s", import.GetErrorString());
		return;
	}
	this->directory = path.ExtractDirName();

	ProcessNode(scene->mRootNode, scene);

}

void Model::LoadMaterial(aiMaterial* mat, aiTextureType type, int meshIndex)
{
	if (mat->GetTextureCount(type) >= 1)
	{
		aiString str;
		if (type == aiTextureType_UNKNOWN)
		{
			mat->GetTexture(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLICROUGHNESS_TEXTURE, &str);
			Util::String file = directory + str.C_Str();
			textures[meshIndex]->AddTexture(TextureIndex::specular0, file.AsCharPtr());
			textures[meshIndex]->AddTexture(TextureIndex::roughness0, file.AsCharPtr());
		}
		else
		{
			mat->GetTexture(type, 0, &str);
			Util::String file = directory + str.C_Str();
			textures[meshIndex]->AddTexture(TextureTypeToTextureIndex(type), file.AsCharPtr());
		}
	}
	else
	{
		switch (type)
		{
		case aiTextureType_DIFFUSE: textures[meshIndex]->AddTexture(TextureIndex::albedo0, "resources/textures/placeholder.png"); break;
		case aiTextureType_NORMALS: textures[meshIndex]->AddTexture(TextureIndex::normal0, "resources/textures/placeholder.png"); break;
		//case aiTextureType_SHININESS: textures[meshIndex]->AddTexture(TextureIndex::roughness0, "resources/textures/white.png"); break;
		//case aiTextureType_SPECULAR: textures[meshIndex]->AddTexture(TextureIndex::specular0, "resources/textures/white.png"); break;
		//case aiTextureType_LIGHTMAP: textures[meshIndex]->AddTexture(Render::TextureIndex::ao0, Render::FrameServer::Instance()->GetSSAOPass()->GetSSAOBuffer()); break;
		default:
			break;
		}
	}
}

void Model::Draw()
{
	for (unsigned int i = 0; i < meshes.Size(); i++)
	{
		if (meshes[i]->IsRenderable())
		{
			//textures[i]->BindTextures();
			meshes[i]->Draw();
		}
			
	}	
}

void Model::DrawDepth()
{
	for (unsigned int i = 0; i < meshes.Size(); i++)
	{
		if (meshes[i]->IsRenderable())
		{
			//textures[i]->BindTextures();
			meshes[i]->Draw();
		}

	}
}

TextureIndex Model::TextureTypeToTextureIndex(const aiTextureType& type)
{
	switch (type)
	{
	case aiTextureType_DIFFUSE: return TextureIndex::albedo0;
	case aiTextureType_NORMALS:	 return TextureIndex::normal0;
	case aiTextureType_SHININESS: return TextureIndex::roughness0;
	case aiTextureType_AMBIENT: return TextureIndex::specular0;
	case aiTextureType_SPECULAR: return TextureIndex::specular0;
	case aiTextureType_LIGHTMAP: return TextureIndex::ao0;
	default: return TextureIndex::albedo0;
	}
	
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	//process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];

		if (mesh->mMaterialIndex >= 0)
		{
			textures.Append(Render::TextureNode::Create());
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			LoadMaterial(material, aiTextureType_DIFFUSE, i);
			LoadMaterial(material, aiTextureType_NORMALS, i);
			LoadMaterial(material, aiTextureType_SHININESS, i);
			LoadMaterial(material, aiTextureType_AMBIENT, i);
			LoadMaterial(material, aiTextureType_SPECULAR, i);
			LoadMaterial(material, aiTextureType_LIGHTMAP, i);
			LoadMaterial(material, aiTextureType_UNKNOWN, i);
		}

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
		if (mesh->HasTextureCoords(0)) // does the mesh contain texture coordinates?
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
	finalMesh->GenerateBuffers();
	return finalMesh;
}
}
