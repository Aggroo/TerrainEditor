#pragma once
#include "GL/glew.h"
#include "foundation/math/vec2.h"
#include "rasterizer.h"
#include <vector>
#include "stb_truetype.h"
#include "core/refcounted.h"

namespace Math
{
    struct textVert
    {
        textVert(vec2 p,vec2 t)
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
		void LoadFromRasterizer(Rasterizer rast);
		void WriteToJPG(const char * filename, int w, int h, void* data, int quality);
		void WriteToPNG(const char * filename, int w, int h, int comp, void* data, int stride_bytes);
		void WriteToTGA(const char * filename, int w, int h, int comp, void* data);
		void bindTex(GLuint slot);

		GLuint GetTextureID() { return m_texture; }

	private:

		GLuint m_texture;
        unsigned char ttf_buffer[1<<20];
        unsigned char temp_bitmap[512*512];
        std::vector<Math::textVert> textMesh;

        stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs
        GLuint ftex;
	};

}
