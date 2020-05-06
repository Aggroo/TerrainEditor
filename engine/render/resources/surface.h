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
struct SamplerParameters;
class TextureNode;
class Model;
class ModelNode;

struct Samplers
{
	inline void ClampBorderBilinear() 
	{
		GLuint samplerID;
		glGenSamplers(1, &samplerID);
		glSamplerParameteri(samplerID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glSamplerParameteri(samplerID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glSamplerParameteri(samplerID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glSamplerParameteri(samplerID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		float borderColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glSamplerParameterfv(samplerID, GL_TEXTURE_BORDER_COLOR, borderColor);
		samplerIDs.Append(samplerID);
	};

	inline void ClampEdgeBilinear()
	{
		GLuint samplerID;
		glGenSamplers(1, &samplerID);
		glSamplerParameteri(samplerID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glSamplerParameteri(samplerID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glSamplerParameteri(samplerID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glSamplerParameteri(samplerID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		samplerIDs.Append(samplerID);
	};

	inline void ClampBorderPoint()
	{
		GLuint samplerID;
		glGenSamplers(1, &samplerID);
		glSamplerParameteri(samplerID, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glSamplerParameteri(samplerID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glSamplerParameteri(samplerID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glSamplerParameteri(samplerID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		float borderColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glSamplerParameterfv(samplerID, GL_TEXTURE_BORDER_COLOR, borderColor);
		samplerIDs.Append(samplerID);
	};

	inline void ClampEdgePoint()
	{
		GLuint samplerID;
		glGenSamplers(1, &samplerID);
		glSamplerParameteri(samplerID, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glSamplerParameteri(samplerID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glSamplerParameteri(samplerID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glSamplerParameteri(samplerID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		samplerIDs.Append(samplerID);
	};

	inline void RepeatBilinear()
	{
		GLuint samplerID;
		glGenSamplers(1, &samplerID);
		glSamplerParameteri(samplerID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glSamplerParameteri(samplerID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glSamplerParameteri(samplerID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glSamplerParameteri(samplerID, GL_TEXTURE_WRAP_T, GL_REPEAT);
		samplerIDs.Append(samplerID);
	};

	inline void RepeatPoint() 
	{
		GLuint samplerID;
		glGenSamplers(1, &samplerID);
		glSamplerParameteri(samplerID, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glSamplerParameteri(samplerID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glSamplerParameteri(samplerID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glSamplerParameteri(samplerID, GL_TEXTURE_WRAP_T, GL_REPEAT);
		samplerIDs.Append(samplerID);
	};

	void BindSamplers()
	{
		glBindSamplers(0, samplerIDs.Size(), &samplerIDs[0]);
	};

	void UnbindSamplers()
	{
		glBindSamplers(0, samplerIDs.Size(), 0);
	};

	Util::Array<GLuint> samplerIDs;
};

class Surface : public Core::RefCounted
{
__DeclareClass(Surface);

public:
    Surface();
    ~Surface();

	void SetupShaderUniforms();
	void BindUniformBuffer();
	SamplerParameters SetupTextureSampler(const Util::String& sampler);

	Render::MaterialParameter* GetParameterByName(const Util::String& name);
	void UpdateParameterByName(const Util::String& name, const Util::Variant& newValue);

	Ptr<Render::TextureNode> GetTextureList() { return textures; }
	Render::Samplers& GetTextureSamplers() { return textureSamplers; }

	void AddModelNode(ModelNode* node);
	bool RemoveModelNode(ModelNode* node);
	const Util::Array<ModelNode*>& GetModelNodes();

	void SetUniformBuffer(Render::UBOInfo* buffer);


private:
	friend class Material;
	friend class ResourceServer;

	GLuint ubo[1];

	void AddParameter(const Util::String &name, const Util::Variant &variable);
	void AddTexture(const Util::String & name, const Util::Variant & variable, const Util::String & textureSample);

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

	Render::Samplers textureSamplers;

};
} // namespace Render
