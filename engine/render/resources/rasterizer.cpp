#include "config.h"
#include "rasterizer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <algorithm>



namespace Math
{
	Rasterizer::Rasterizer()
	{
		Color clr;
		clr.r = 0;
		clr.g = 0;
		clr.b = 0;

		pixels.assign(width*height, clr);
		zDepth.assign(width*height, 0.0f);
			
	}

	Rasterizer::~Rasterizer()
	{
		
	}
	
	Rasterizer::Scanline::Scanline(int x1, int x2, int y)
	{
		if (x1 > x2)
		{
			startX = x2, endX = x1;
		}
		else
		{
			startX = x1, endX = x2;
		}
		Y = y;
	}

	Rasterizer::Edge::Edge(vec4 v1, vec4 v2)
	{
		if (v1.y() < v2.y())
		{
			xb = v1.x();
			yb = v1.y();
			xe = v2.x();
			ye = v2.y();
		}
		else
		{
			xb = v2.x();
			yb = v2.y();
			xe = v1.x();
			ye = v1.y();
		}

		dx = (xe - xb);
		dy = (ye - yb);

		if (dx >= 0)
		{
			if (dy > dx)
			{
				oct = Octant2;
				std::swap(dx, dy);
			}
			else
				oct = Octant1;
		}
		else
		{
			if (dy > -dx)
			{
				oct = Octant3;
				std::swap(dx, dy);
			}
			else
				oct = Octant4;
		}

		dx = abs(dx);
		dy = abs(dy);

		d = 2 * dy - dx;
		e = 2 * dy;
		ne = 2 * (dy - dx);

		x = xb; y = yb;

	}

	void Rasterizer::scanline(const Scanline &scanl)
	{
		int xdiff = scanl.endX - scanl.startX;

		if (xdiff == 0)
			return;

		for (int x = scanl.startX; x <= scanl.endX; x++)
		{
			float u, v, w;
			barycentric(vec2(x,scanl.Y), vec2(vertex1.x(),vertex1.y()), vec2(vertex2.x(),vertex2.y()), vec2(vertex3.x(),vertex3.y()), u, v, w);
			float corrW = (u*w1 + v*w2 + w*w3);
			float pixelZ = ((vertex1.z()*u) + (vertex2.z()*v) + (vertex3.z()*w));
			

			int index = (scanl.Y*width + x);

			if (index < 0 || index >= (width*height))
				continue;
			if (zDepth[index] > pixelZ)
				continue;

			zDepth[index] = pixelZ;

			
			vec2 tex = ((uv1*u)*w1 + (uv2*v)*w2 + (uv3*w)*w3)/corrW;
			vec3 norm = ((norm1*u)*w1 + (norm2*v)*w2 + (norm3*w)*w3)/corrW;
			vec3 diff = ((diffuseColor1*u)*w1 + (diffuseColor2*v)*w2 + (diffuseColor3*w)*w3)/corrW;

			Color clr = fragments({ tex[0], 1 - tex[1] }, norm, image, this->w, h, n);
			
			clr.r = std::min((clr.r * diff.x()), 255.0f);
			clr.g = std::min((clr.g * diff.y()), 255.0f);
			clr.b = std::min((clr.b * diff.z()), 255.0f);

			if ((scanl.Y*width + x) < 0)
				continue;
			else if ((scanl.Y*width + x) >= width*height)
				continue;

			if (x > 0 && x < width)
				this->pixels[index] = clr;

		}
	}

	void Rasterizer::increment(Edge &edge)
	{
		if (edge.oct == Octant1)
		{
			edge.x++;
			if (edge.d < 0)
			{
				edge.d += edge.e;
			}
			else
			{
				edge.d += edge.ne;
				edge.y++;
			}
			
		}
		else if (edge.oct == Octant2)
		{
			edge.y++;
			if (edge.d < 0)
			{
				edge.d += edge.e;
			}
			else
			{
				edge.d += edge.ne;
				edge.x++;
			}
			
		}
		else if (edge.oct == Octant3)
		{
			edge.y++;
			if (edge.d < 0)
			{
				edge.d += edge.e;
			}
			else
			{
				edge.d += edge.ne;
				edge.x--;
			}
			
		}
		else if (edge.oct == Octant4)
		{
			edge.x--;
			if (edge.d < 0)
			{
				edge.d += edge.e;
			}
			else
			{
				edge.d += edge.ne;
				edge.y++;
			}
			
		}
	}

	std::vector<Rasterizer::Color> Rasterizer::getPixels()
	{
		return this->pixels;
	}

	void Rasterizer::rasterize(vec4 v1, vec4 v2, vec4 v3)
	{
		Edge edges[3] = {
			Edge(v1, v2),
			Edge(v1, v3),
			Edge(v3, v2)
		};

		int maxLength = 0, longEdge = 0;

		for (int i = 0; i < 3; i++) {
			int length = edges[i].ye - edges[i].yb;
			if (length > maxLength) {
				maxLength = length;
				longEdge = i;
			}
		}

		int shortEdge1, shortEdge2;

		if (edges[(longEdge + 1) % 3].yb > edges[(longEdge + 2) % 3].yb)
		{
			shortEdge2 = (longEdge + 1) % 3;
			shortEdge1 = (longEdge + 2) % 3;
		}
		else
		{
			shortEdge1 = (longEdge + 1) % 3;
			shortEdge2 = (longEdge + 2) % 3;
		}

		while (edges[longEdge].y < edges[longEdge].ye)
		{
			int oldy = edges[longEdge].y;
			while (edges[longEdge].y == oldy)
				increment(edges[longEdge]);

			if (edges[longEdge].y <= edges[shortEdge1].ye)
			{
				while (edges[longEdge].y > edges[shortEdge1].y)
					increment(edges[shortEdge1]);

				scanline(Scanline(edges[shortEdge1].x, edges[longEdge].x, edges[longEdge].y));
			}
			else
			{
				while (edges[longEdge].y > edges[shortEdge2].y)
					increment(edges[shortEdge2]);
					
				scanline(Scanline(edges[shortEdge2].x, edges[longEdge].x, edges[longEdge].y));
					
			}
		}
		
	}

	void Rasterizer::barycentric(vec2 p, vec2 a, vec2 b, vec2 c, float &u, float &v, float &w)
	{
		vec2 v0 = b - a, v1 = c - a, v2 = p - a;
		float d00 = v0.dotProd(v0);
		float d01 = v0.dotProd(v1);
		float d11 = v1.dotProd(v1);
		float d20 = v2.dotProd(v0);
		float d21 = v2.dotProd(v1);
		float denom = d00 * d11 - d01 * d01;
		v = (d11 * d20 - d01 * d21) / denom;
		w = (d00 * d21 - d01 * d20) / denom;
		u = 1.0f - v - w;
	}

	void Rasterizer::draw(mat4 pMat, mat4 tMat)
	{
		meshData = mesh->GetMesh();
		indices = mesh->getIndices();

		for (int i = 0; i < indices.size(); i += 3)
		{
			vec4 v1(meshData[indices[i]].pos.x(), meshData[indices[i]].pos.y(), meshData[indices[i]].pos.z());
			vec4 v2(meshData[indices[i + 1]].pos.x(), meshData[indices[i + 1]].pos.y(), meshData[indices[i + 1]].pos.z());
			vec4 v3(meshData[indices[i + 2]].pos.x(), meshData[indices[i + 2]].pos.y(), meshData[indices[i + 2]].pos.z());

			vec2 u1(meshData[indices[i]].uv.x(), meshData[indices[i]].uv.y());
			vec2 u2(meshData[indices[i + 1]].uv.x(), meshData[indices[i + 1]].uv.y());
			vec2 u3(meshData[indices[i + 2]].uv.x(), meshData[indices[i + 2]].uv.y());

 			vec3 n1(meshData[indices[i]].norm.x(), meshData[indices[i]].norm.y(), meshData[indices[i]].norm.z());
			vec3 n2(meshData[indices[i + 1]].norm.x(), meshData[indices[i + 1]].norm.y(), meshData[indices[i + 1]].norm.z());
			vec3 n3(meshData[indices[i + 2]].norm.x(), meshData[indices[i + 2]].norm.y(), meshData[indices[i + 2]].norm.z());

			vertex1 = pMat*tMat*v1;
			vertex2 = pMat*tMat*v2;
			vertex3 = pMat*tMat*v3;

			perspective(vertex1, vertex2, vertex3);
			
			uv1 = u1;
			uv2 = u2;
			uv3 = u3;

			norm1 = n1;
			norm2 = n2;
			norm3 = n3;

			diffuseColor1 = vertShader(meshData[indices[i]].pos, meshData[indices[i]].norm, tMat);
			diffuseColor2 = vertShader(meshData[indices[i+1]].pos, meshData[indices[i+1]].norm, tMat);
			diffuseColor3 = vertShader(meshData[indices[i+2]].pos, meshData[indices[i+2]].norm, tMat);

			rasterize(vertex1, vertex2, vertex3);
		}
	}

	void Rasterizer::fragment(const std::function<Color(vec2 tex, vec3 norm, unsigned char* image, int w, int h, int n)>& func)
	{
		this->fragments = func;
	}



	void Rasterizer::vertexShader(const std::function<vec3(vec3 pos, vec3 norm, mat4 nMat)>& func)
	{
		this->vertShader = func;
	}

	void Rasterizer::getTexture(const char* filename)
	{
		this->image = stbi_load(filename, &w, &h, &n, 0);
	}

	void Rasterizer::perspective(vec4& v1, vec4& v2, vec4& v3)
	{
		w1 = 1 / v1.w();
		w2 = 1 / v2.w();
		w3 = 1 / v3.w();

		v1[0] = ((v1.x() * w1 + 1) / 2) * width;
		v1[1] = ((v1.y() * w1 + 1) / 2) * height;
		v1[2] = v1.z() * w1;

		v2[0] = ((v2[0] * w2 + 1) / 2) * width;
		v2[1] = ((v2[1] * w2 + 1) / 2) * height;
		v2[2] = v2[2] * w2;
		
		v3[0] = ((v3[0] * w3 + 1) / 2) * width;
		v3[1] = ((v3[1] * w3 + 1) / 2) * height;
		v3[2] = v3[2] * w3;
	}

	void Rasterizer::getMesh(std::shared_ptr<MeshResources> mesh)
	{
		this->mesh = mesh;
	}

	void Rasterizer::clear()
	{
		memset(&pixels[0], 0, sizeof(pixels[0]) * pixels.size());
		zDepth.assign(width*height,0);
	}

	

}