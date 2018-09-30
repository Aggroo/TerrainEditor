#include "config.h"
#include "textureresource.h"
#include <iostream>
#include <fstream>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"


namespace Render
{
__ImplementClass(Render::TextureResource, 'TXTR', Core::RefCounted);
TextureResource::TextureResource()
{

}

TextureResource::~TextureResource()
{
	glDeleteTextures(1, &m_texture);
}

void TextureResource::LoadTextureFile(const char * filename)
{
	int w, h, n;
	unsigned char *image = stbi_load(filename, &w, &h, &n, 0);

	if (image == nullptr)
		throw(std::string("Failed to load texture"));

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (n == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	else if (n == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	glGenerateMipmap(GL_TEXTURE_2D);
	glGenerateTextureMipmap(m_texture);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(image);

}

void TextureResource::LoadCubemap(Util::Array<Util::String> texures)
{
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < texures.Size(); i++)
	{
		unsigned char *data = stbi_load(texures[i].AsCharPtr(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			printf("Cubemap texture failed to load at path: %s", texures[i].AsCharPtr());
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void TextureResource::LoadFromRasterizer(Math::Rasterizer rast)
{
	//stbi_write_bmp("blorf2.bmp", rast.width, rast.height, 3, &rast.getPixels()[0]);
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, rast.width, rast.height, 0, GL_RGB, GL_UNSIGNED_BYTE, &rast.getPixels()[0]);

	//glGenerateMipmap(GL_TEXTURE_2D);
	//glGenerateTextureMipmap(m_texture);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureResource::WriteToJPG(const char * filename, int w, int h, void* data, int quality)
{
	stbi_write_jpg(filename, w, h, 3, data, quality);
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
	glBindTexture(GL_TEXTURE_2D, m_texture);
}

void TextureResource::bindCubeTex(GLuint slot) const
{
	glActiveTexture(GL_TEXTURE0+slot);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
}

void TextureResource::unbindTex() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}


}