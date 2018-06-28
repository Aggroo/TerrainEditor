#pragma once
#include "foundation/math/vec2.h"
#include "foundation/math/vec3.h"
#include "foundation/math/vec4.h"
#include "foundation/math/mat4.h"
#include "meshresource.h"
#include "stb_image.h"
#include <vector>
#include <memory>
#include <functional>


namespace Math
{
	class Rasterizer
	{
	public:
		Rasterizer();
		~Rasterizer();

		const int width = 512;
		const int height = 512;

		enum Octant
		{
			Octant1,
			Octant2,
			Octant3,
			Octant4
		};

		struct Color
		{
			unsigned char r = 0;
			unsigned char g = 0;
			unsigned char b = 0;
		};


		class Scanline
		{
		public:
			Scanline(int x1, int x2, int y);
			int startX, endX, Y;
		};

		class Edge
		{
		public:
			Edge(vec4 v1, vec4 v2);

			Octant oct;
			int x, y, xb, xe, yb, ye, d, dy, dx, e, ne;
		};

		
		void scanline(const Scanline &scanl);
		void increment(Edge &edge);
		std::vector<Rasterizer::Color> getPixels();

		void rasterize(vec4 v1, vec4 v2, vec4 v3);
		void barycentric(vec2 p, vec2 a, vec2 b, vec2 c, float &u, float &v, float &w);
		void draw(mat4, mat4);
		void fragment(const std::function<Color(vec2 tex, vec3 norm, unsigned char* image, int w, int h, int n)>& func);
		void vertexShader(const std::function<vec3(vec3 pos, vec3 norm, mat4 nMat)>& func);
		void getTexture(const char* filename);

		void perspective(vec4&,vec4&,vec4&);
		
		void getMesh(std::shared_ptr<Render::MeshResources> mesh);
		void clear();

	private:
		
		int leftY, botY, rightY, leftX, botX, rightX;
		vec4 vertex1, vertex2, vertex3;
		vec2 uv1, uv2, uv3;
		vec3 norm1, norm2, norm3;
		vec3 diffuseColor1, diffuseColor2, diffuseColor3;
		std::vector<Color> pixels;
		Util::Array<Render::Vertex> meshData;
		Util::Array<GLuint> indices;
		std::shared_ptr<Render::MeshResources> mesh;
		std::function<Color(vec2 tex, vec3 norm, unsigned char* image, int w, int h, int n)> fragments;
		std::function<vec3(vec3 pos, vec3 norm, mat4 nMat)> vertShader;

		std::vector<float> zDepth;
		int w, h, n;
		float w1, w2, w3;
		unsigned char* image;
		
	};

}