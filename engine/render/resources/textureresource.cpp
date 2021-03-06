#include "config.h"
#include "textureresource.h"
#include <iostream>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
#define STB_TRUETYPE_IMPLEMENTATION


namespace Render
{
__ImplementClass(Render::TextureResource, 'TXTR', Core::RefCounted)
TextureResource::TextureResource() : isHDR(false), channels(1), height(0), image(0) 
{

}

TextureResource::~TextureResource()
{
	glDeleteTextures(1, &textureID);
}

void TextureResource::LoadTextureFile(const char * filename)
{
	if(stbi_is_hdr(filename))
	{
		float* pixels = stbi_loadf(filename, &width, &height, &channels, 3);
		if (pixels)
		{
			image = (reinterpret_cast<unsigned char*>(pixels));
			isHDR = true;
		}
	}
	else
	{
		unsigned char* pixels = stbi_load(filename, &width, &height, &channels, 0);

		if (pixels)
			image = (pixels);
	}

	if (image == nullptr)
	{
		T_CORE_ERROR("Couldn't load texture file {0}", filename);
		assert(false);
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if(channels == 1)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, pixels<unsigned char>());
	else if (channels == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels<unsigned char>());
	else if (channels == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels<unsigned char>());

	glGenerateMipmap(GL_TEXTURE_2D);
	glGenerateTextureMipmap(textureID);

	glBindTexture(GL_TEXTURE_2D, 0);

	if(!isHDR)
		stbi_image_free(image);

}

void TextureResource::LoadTextureFile(const CreateTextureParameters& textureVariables)
{
	if (stbi_is_hdr(textureVariables.filename))
	{
		float* pixels = stbi_loadf(textureVariables.filename, &width, &height, &channels, 3);
		if (pixels)
		{
			image = (reinterpret_cast<unsigned char*>(pixels));
			isHDR = true;
		}
	}
	else
	{
		unsigned char* pixels = stbi_load(textureVariables.filename, &width, &height, &channels, 0);

		if (pixels)
			image = (pixels);
	}

	if (image == nullptr)
	{
		T_CORE_ERROR("Failed to load texture {0}", textureVariables.filename);
		_assert(false);
	}

	this->textureParameters = textureVariables;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	float borderColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	switch (textureVariables.sampler.minFilter)
	{
	case TextureFilterMode::Nearest: glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); break;
	case TextureFilterMode::Linear: glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); break;
	case TextureFilterMode::LinearMipmapLinear: glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); break;
	case TextureFilterMode::LinearMipmapNearest: glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); break;
	case TextureFilterMode::NearestMipmapLinear: glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR); break;
	case TextureFilterMode::NearestMipmapNearest: glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST); break;
	default:
		T_CORE_ERROR("TEXTURE ERROR: Invalid min filter");
		_assert(false);
	}

	switch (textureVariables.sampler.maxFilter)
	{
	case TextureFilterMode::Nearest: glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); break;
	case TextureFilterMode::Linear: glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); break;
	case TextureFilterMode::LinearMipmapLinear: glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); break;
	case TextureFilterMode::LinearMipmapNearest: glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST); break;
	case TextureFilterMode::NearestMipmapLinear: glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR); break;
	case TextureFilterMode::NearestMipmapNearest: glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST); break;
	default:
		T_CORE_ERROR("TEXTURE ERROR: Invalid max filter");
		_assert(false);
	}

	switch (textureVariables.sampler.wrapU)
	{
	case TextureWrapMode::ClampToEdge: glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); break;
	case TextureWrapMode::Repeat: glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); break;
	case TextureWrapMode::MirroredRepeat: glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); break;
	case TextureWrapMode::ClampToBorder: glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); 
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		break;
	default:
		T_CORE_ERROR("TEXTURE ERROR: Invalid wrap s filter");
		_assert(false);
	}

	switch (textureVariables.sampler.wrapV)
	{
	case TextureWrapMode::ClampToEdge: glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); break;
	case TextureWrapMode::Repeat: glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); break;
	case TextureWrapMode::MirroredRepeat: glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT); break;
	case TextureWrapMode::ClampToBorder: glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		break;
	default:
		T_CORE_ERROR("TEXTURE ERROR: Invalid wrap t filter");
		_assert(false);
	}

	if (channels == 1)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, pixels<unsigned char>());
	else if (channels == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels<unsigned char>());
	else if (channels == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels<unsigned char>());

	glGenerateMipmap(GL_TEXTURE_2D);
	glGenerateTextureMipmap(textureID);

	glBindTexture(GL_TEXTURE_2D, 0);

	if (!isHDR)
		stbi_image_free(image);
}

void TextureResource::LoadCubemap(Util::Array<Util::String> textures)
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	for (unsigned int i = 0; i < textures.Size(); i++)
	{
		unsigned char *data = stbi_load(textures[i].AsCharPtr(), &width, &height, &channels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			T_CORE_ERROR("Cubemap texture failed to load at path: {0}", textures[i].AsCharPtr());
			stbi_image_free(data);
		}
	}

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glGenerateTextureMipmap(textureID);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void TextureResource::LoadFromRasterizer(Math::Rasterizer rast)
{
	//stbi_write_bmp("blorf2.bmp", rast.width, rast.height, 3, &rast.getPixels()[0]);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, rast.width, rast.height, 0, GL_RGB, GL_UNSIGNED_BYTE, &rast.getPixels()[0]);

	//glGenerateMipmap(GL_TEXTURE_2D);
	//glGenerateTextureMipmap(m_texture);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureResource::WriteToJPG(const char * filename, int w, int h, void* data, int quality)
{
	stbi_write_jpg(filename, w, h, 4, data, quality);
}

void TextureResource::WriteToPNG(const char * filename, int w, int h, int comp, void* data, int stride_bytes)
{
	stbi_write_png(filename, w, h, comp, data, stride_bytes);
}

void TextureResource::WriteToTGA(const char * filename, int w, int h, int comp, void* data)
{
	stbi_write_tga(filename, w, h, comp, data);
}

void TextureResource::bindTex(GLuint slot) const
{
	glActiveTexture(GL_TEXTURE0+slot);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void TextureResource::bindCubeTex(GLuint slot) const
{
	glActiveTexture(GL_TEXTURE0+slot);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
}

void TextureResource::unbindTex() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}


}