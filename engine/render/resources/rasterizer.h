#pragma once
#include "foundation/math/vector2D.h"
#include "foundation/math/vector3D.h"
#include "foundation/math/vector4D.h"
#include "foundation/math/matrix4D.h"
#include "meshresource.h"
#include "stb_image.h"
#include <vector>
#include <memory>
#include <functional>


namespace CGMath
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
			Edge(vector4D v1, vector4D v2);

			Octant oct;
			int x, y, xb, xe, yb, ye, d, dy, dx, e, ne;
		};

		
		void scanline(const Scanline &scanl);
		void increment(Edge &edge);
		std::vector<Rasterizer::Color> getPixels();

		void rasterize(vector4D v1, vector4D v2, vector4D v3);
		void barycentric(vector2D p, vector2D a, vector2D b, vector2D c, float &u, float &v, float &w);
		void draw(matrix4D, matrix4D);
		void fragment(const std::function<Color(vector2D tex, vector3D norm, unsigned char* image, int w, int h, int n)>& func);
		void vertexShader(const std::function<vector3D(vector3D pos, vector3D norm, matrix4D nMat)>& func);
		void getTexture(const char* filename);

		void perspective(vector4D&,vector4D&,vector4D&);
		
		void getMesh(std::shared_ptr<MeshResources> mesh);
		void clear();

	private:
		
		int leftY, botY, rightY, leftX, botX, rightX;
		vector4D vertex1, vertex2, vertex3;
		vector2D uv1, uv2, uv3;
		vector3D norm1, norm2, norm3;
		vector3D diffuseColor1, diffuseColor2, diffuseColor3;
		std::vector<Color> pixels;
		std::vector<CGMath::Vertex> meshData;
		std::vector<GLuint> indices;
		std::shared_ptr<MeshResources> mesh;
		std::function<Color(vector2D tex, vector3D norm, unsigned char* image, int w, int h, int n)> fragments;
		std::function<vector3D(vector3D pos, vector3D norm, matrix4D nMat)> vertShader;

		std::vector<float> zDepth;
		int w, h, n;
		float w1, w2, w3;
		unsigned char* image;
		
	};

}