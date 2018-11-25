#include "config.h"
#include "entity.h"
#include "render/server/shaderserver.h"
#include "render/camera/camera.h"

namespace Game
{
__ImplementClass(Game::Entity, 'ENTY', Game::EntityBase);
Entity::Entity() : entityName("")
{
	this->mesh = Render::Model::Create();
	this->shader = Render::ShaderObject::Create();
	this->textures = Render::TextureNode::Create();
}

Entity::~Entity()
{
}

void Entity::Activate()
{
	this->shader->BindProgram();
	this->shader->setupUniformInt("AlbedoMap", (GLuint)Render::TextureIndex::albedo0);
	this->shader->setupUniformInt("NormalMap", (GLuint)Render::TextureIndex::normal0);
	this->shader->setupUniformInt("SpecularMap", (GLuint)Render::TextureIndex::specular0);
	this->shader->setupUniformInt("RoughnessMap", (GLuint)Render::TextureIndex::roughness0);
	this->shader->setupUniformInt("aoMap", (GLuint)Render::TextureIndex::ao0);

	EntityBase::Activate();
}

void Entity::Deactivate()
{
	EntityBase::Deactivate();
}

void Entity::Update()
{
	this->shader->BindProgram();
	this->textures->BindTextures();
	this->shader->setupMatrix4fv("Model", this->transform);

	//if (this->mesh->IsRenderable())
	this->mesh->Draw();
}

void Entity::OnUI()
{
}

void Entity::SetMesh(Util::String filename)
{
	this->mesh->LoadModel(filename.AsCharPtr());
}

Ptr<Render::Model> Entity::GetMesh()
{
	return this->mesh;
}

void Entity::SetTextures(Util::String albedo, Util::String normal, Util::String metallic, Util::String roughness, Util::String ao)
{
	//TODO: FIX THIS WHEN SHADER SETTINGS ARE HANDLED IN FILES
	this->textures->AddTexture(Render::TextureIndex::albedo0, albedo.AsCharPtr());
	this->textures->AddTexture(Render::TextureIndex::normal0, normal.AsCharPtr());
	this->textures->AddTexture(Render::TextureIndex::specular0, metallic.AsCharPtr());
	this->textures->AddTexture(Render::TextureIndex::roughness0, roughness.AsCharPtr());
	if (ao == "Default")
	{
		this->textures->AddTexture(Render::TextureIndex::ao0, "resources/textures/terrain_textures/default/defaultAO.png");
	}
	else
	{
		this->textures->AddTexture(Render::TextureIndex::ao0, ao.AsCharPtr());
	}
}

void Entity::SetShaders(Util::String vertexShader, Util::String fragmentShader, const char* name)
{
	GLuint vert = Render::ShaderServer::Instance()->LoadVertexShader(vertexShader);
	GLuint frag = Render::ShaderServer::Instance()->LoadFragmentShader(fragmentShader);

	this->shader->AddShader(vert);
	this->shader->AddShader(frag);
	this->shader->LinkShaders();

	Render::ShaderServer::Instance()->AddShaderObject(name, this->shader);
}

Ptr<Render::ShaderObject> Entity::GetShader()
{
	return this->shader;
}

void Entity::SetName(Util::String name)
{
	this->entityName = name;
}

Util::String Entity::GetName()
{
	return entityName;
}

void Entity::SetIBLMaps(Ptr<Render::TextureResource> envmapID, Ptr<Render::TextureResource> irID, Ptr<Render::TextureResource> brdfID)
{
	this->environmentMap = envmapID;
	textures->AddTexture(Render::TextureIndex::environmentMap, this->environmentMap);
	this->shader->setupUniformInt("environmentMap", (GLuint)Render::TextureIndex::environmentMap);

	this->irradiance = irID;
	textures->AddTexture(Render::TextureIndex::irradiance, this->irradiance);
	this->shader->setupUniformInt("irradiance", (GLuint)Render::TextureIndex::irradiance);

	this->brdf = brdfID;
	textures->AddTexture(Render::TextureIndex::brdf, this->brdf);
	this->shader->setupUniformInt("brdfLUT", (GLuint)Render::TextureIndex::brdf);
}

Ptr<Render::TextureResource> Entity::GetEnvironmentMap()
{
	return this->environmentMap;
}
}
