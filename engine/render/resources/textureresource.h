#pragma once
#include "GL/glew.h"
#include "foundation/math/vector2D.h"
#include "rasterizer.h"
#include <vector>
#include "stb_truetype.h"

namespace CGMath
{
    struct textVert
    {
        textVert(vector2D p,vector2D t)
        {
            pos = p;
            tex = t;
        }
        CGMath::vector2D pos;
        CGMath::vector2D tex;
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
        std::vector<CGMath::textVert> textMesh;

        stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs
        GLuint ftex;
	};

}