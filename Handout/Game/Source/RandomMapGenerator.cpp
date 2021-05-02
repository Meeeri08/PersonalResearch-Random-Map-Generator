#include "RandomMapGenerator.h"
#include "App.h"
#include "Map.h"
#include "Log.h"

RandomMapGenerator::RandomMapGenerator()
{
	name.create("map_generator");
}

RandomMapGenerator::~RandomMapGenerator()
{
}

bool RandomMapGenerator::Awake() 
{

	LOG("Loading Procedural Map Data");
	bool ret = true;



	return ret;
}

int RandomMapGenerator::generateSeed() 
{

	msec = time(NULL) * 1000;

	return msec;
}

void RandomMapGenerator::generateNoise() 
{

	generateNoise(default_seed);
}

void RandomMapGenerator::generateNoise(unsigned int seed)
{

	FastNoiseLite noise;

	noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin); 

	noise.SetSeed(seed);

	noise.SetFrequency(0.05);

	for (int x = 0; x < 100; x++)
	{
		for (int y = 0; y < 100; y++)
		{

			app->map->height_map[x][y] = (noise.GetNoise((float)x, (float)y) +1) * 0.5;

		}
	}
}
