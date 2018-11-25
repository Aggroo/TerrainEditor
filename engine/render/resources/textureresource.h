#pragma once
#include "GL/glew.h"
#include "foundation/math/vec2.h"
#include "rasterizer.h"
#include <vector>
#include "stb_truetype.h"
#include "core/refcounted.h"

namespace Render
{
struct textVert
{
    textVert(Math::vec2 p, Math::vec2 t)
    {
        pos = p;
        tex = t;
    }
    Math::vec2 pos;
    Math::vec2 tex;
};

class TextureResource : public Core::RefCounted
{
__DeclareClass(TextureResource)
public:
	TextureResource();
	~TextureResource();

	void LoadTextureFile(const char * filename);
	void LoadCubemap(Util::Array<Util::String> texures);
	void LoadFromRasterizer(Math::Rasterizer rast);
	void WriteToJPG(const char * filename, int w, int h, void* data, int quality);
	void WriteToPNG(const char * filename, int w, int h, int comp, void* data, int stride_bytes);
	void WriteToTGA(const char * filename, int w, int h, int comp, void* data);
	void bindTex(GLuint slot) const;
	void bindCubeTex(GLuint slot) const;
	void unbindTex() const;

	GLuint& GetTextureID() { return m_texture; }
	int& GetWidth() { return width; }
	int& GetHeight() { return height; }
	int& GetChannels() { return channels; }
	bool& IsHDR() { return isHDR; }

	template<typename T>
	const T* pixels() const
	{
		return reinterpret_cast<const T*>(image);
	}

private:

	GLuint m_texture;
	bool isHDR;
	int width, height, channels;
	unsigned char* image;

};

}
