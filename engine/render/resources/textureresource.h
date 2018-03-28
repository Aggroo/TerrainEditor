#pragma once
#include "GL/glew.h"
#include "foundation/math/vec2.h"
#include "rasterizer.h"
#include <vector>
#include "stb_truetype.h"

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

	class TextureResource
    {
	public:
		TextureResource();
		~TextureResource();

		void LoadTextureFile(const char * filename);
		void LoadFromRasterizer(Rasterizer rast);
		void bindTex(GLuint slot);

	private:

		GLuint m_texture;
        unsigned char ttf_buffer[1<<20];
        unsigned char temp_bitmap[512*512];
        std::vector<Math::textVert> textMesh;

        stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs
        GLuint ftex;
	};

}