#pragma once
//------------------------------------------------------------------------------
/**
    Class to handle the surface of a mesh

    (C) 2020 Anton Grönberg
*/
//------------------------------------------------------------------------------
#include "core/refcounted.h"
#include "GL/glew.h"

namespace Util 
{
class Variant;
}
namespace Render
{
struct MaterialParameter;
struct UBOInfo;
class TextureNode;
class Model;
class ModelNode;

class Surface : public Core::RefCounted
{
__DeclareClass(Surface);

public:
    Surface();
    ~Surface();

	void SetupShaderUniforms();
	void BindUniformBuffer();

	Render::MaterialParameter* GetParameterByName(const Util::String& name);
	void UpdateParameterByName(const Util::String& name, const Util::Variant& newValue);

	Ptr<Render::TextureNode> GetTextureList() { return textures; }

	void AddModelNode(ModelNode* node);
	bool RemoveModelNode(ModelNode* node);
	const Util::Array<ModelNode*>& GetModelNodes();

	void SetUniformBuffer(Render::UBOInfo* buffer);

private:
	friend class Material;
	friend class ResourceServer;

	GLuint ubo[1];

	void AddParameter(const Util::String &name, const Util::Variant &variable);

	Util::String name;
	Util::String filepath;

	///The material this surface is using
	Ptr<Render::Material> material;

	///This surface loaded textures
    Ptr<Render::TextureNode> textures;

	///Loaded material Parameters
	Util::Dictionary<Util::String, MaterialParameter*> parametersByName;
	Util::Array<MaterialParameter*> parameters;

	Util::Array<ModelNode*> modelNodes;

	Render::UBOInfo* uboBuffer;
	bool uniformBufferExist;

};
} // namespace Render
