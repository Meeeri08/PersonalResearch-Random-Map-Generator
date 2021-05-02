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

	//TODO 1: 
	//Create a FastNoiseLite object.
	

	//TODO 1.2: 
	//Now that you have a FastNoiseLite object, the 
	//next step is to set its Noise type to Perlin noise.


	//TODO 1.3:
	//In order to create the noise you need to set the
	//seed. Uncomment de following line and change 
	//"myObject" to your variable name.
	//myObject.SetSeed(seed);


	//TODO 1.4:
	//Now that you have a Perlin Noise object and the seed,
	//we need to set the frequency. For this time we are going
	//to set the frequency to 0.05.
	

	for (int x = 0; x < 100; x++)
	{
		for (int y = 0; y < 100; y++)
		{

			//TODO 2:
			//In order to store the values generated by Perlin Noise you
			//have to get noise at the coords [x][y] and store that value 
			//in app->map->height_map.
			//TODO 2.1:
			//Noise must be always between 1 and 0, so the next step you have
			//to do is to apply the formula given by the author of the library: (Noise + 1) *0,5.



			//Press [F5] to see your generated perlin noise!
		}
	}
}
