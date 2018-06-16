#pragma once
#include "core/refcounted.h"
#include "PerlinNoise.hpp"
#include "render/resources/textureresource.h"

namespace Generator
{
struct Color
{
	unsigned char r;
	unsigned char g;
	unsigned char b;

	Color()
	{
		r = 255;
		g = 255;
		b = 255;
	}

	Color& operator =(const float& a)
	{
		r = a*255;
		g = a*255;
		b = a*255; 
		return *this;
	}
};

class PerlinNoiseGenerator : Core::RefCounted
{
__DeclareClass(PerlinNoiseGenerator)

public:
	PerlinNoiseGenerator();
	~PerlinNoiseGenerator();

	/** GenerateNoiseMap
		Generates a perlin noise texture

		@param filename file the texture is written to
		@param mapWidth width of texture
		@param mapHeight height of texture
		@param scale scales the values for the perlin noise generation
		@param octaves number of octaves in the perlin noise
		@param persistance persistance value of perlin noise
		@param lacunarity lacunarity value of perlin noise
		@return boolean result
	*/
	void GenerateNoiseMap(const char* filename, int mapWidth, int mapHeight, float scale, int octaves, float persistance, float lacunarity);

	Ptr<Math::TextureResource> GetTexture() { return texture; }

private:

	siv::PerlinNoise perlin;

	float* noiseMap;

	Ptr<Math::TextureResource> texture;

	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen; //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> dis;
};

}
