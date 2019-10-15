#include "config.h"
#include "perlinnoisegenerator.h"
#include "foundation/math/math.h"


namespace Generator
{
__ImplementClass(Generator::PerlinNoiseGenerator, 'GEPN', Core::RefCounted)

PerlinNoiseGenerator::PerlinNoiseGenerator() : gen(rd()), dis(0, 3000), noiseMap(nullptr)
{
	texture = Render::TextureResource::Create();
}

PerlinNoiseGenerator::~PerlinNoiseGenerator()
{
	delete noiseMap;
}

void PerlinNoiseGenerator::GenerateNoiseMap(const char* filename, int mapWidth, int mapHeight, float scale, int octaves, float persistance,
	float lacunarity)
{
	noiseMap = new float[mapWidth*mapHeight];

	perlin.reseed(dis(gen));

	if(scale <= 0)
	{
		scale = 0.00001f;
	}

	float maxNoiseHeight = FLT_MIN;
	float minNoiseHeight = FLT_MAX;

	for (int y = 0; y < mapHeight; ++y)
	{
		for (int x = 0; x < mapWidth; ++x)
		{
			float amplitude = 1;
			float frequency = 1;
			float noiseHeight = 0;

			for (int i = 0; i < octaves; ++i)
			{
				float sampleX = x / scale * frequency;
				float sampleY = y / scale * frequency;

				float perlinValue = perlin.octaveNoise0_1(sampleX, sampleY, octaves) * 2 - 1;
				noiseHeight += (abs(perlinValue)*-1) * amplitude;

				amplitude *= persistance;
				frequency *= lacunarity;
			}

			if(noiseHeight > maxNoiseHeight)
			{
				maxNoiseHeight = noiseHeight;
			}
			else if(noiseHeight < minNoiseHeight)
			{
				minNoiseHeight = noiseHeight;
			}

			noiseMap[y * mapWidth + x] = noiseHeight;
		}
	}
	for (int y = 0; y < mapHeight; ++y)
	{
		for (int x = 0; x < mapWidth; ++x)
		{
			noiseMap[y * mapWidth + x] = Math::InverseLerp(minNoiseHeight, maxNoiseHeight, noiseMap[y * mapWidth + x]);
		}
	}

	Color* colorMap = new Color[mapWidth*mapHeight];

	for (int y = 0; y < mapHeight; ++y)
	{
		for (int x = 0; x < mapWidth; ++x)
		{
			colorMap[y * mapWidth + x] = Math::Lerp(0.0f, 1.0f, noiseMap[y * mapWidth + x]);
		}
	}

	texture->WriteToJPG(filename, mapWidth, mapHeight, (void*)colorMap, 100);
}
}
