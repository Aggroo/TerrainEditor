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


namespace Math
{
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

	void TextureResource::LoadFromRasterizer(Rasterizer rast)
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

	void TextureResource::bindTex(GLuint slot)
	{
		glActiveTexture(GL_TEXTURE0+slot);
		glBindTexture(GL_TEXTURE_2D, m_texture);
	}


}