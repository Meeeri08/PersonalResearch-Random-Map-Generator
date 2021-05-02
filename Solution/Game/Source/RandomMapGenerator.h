#ifndef __RandomMapGenerator__
#define __RandomMapGenerator__

#include "Module.h"
#include <time.h>
#include "FastNoiseLite.h"

class RandomMapGenerator : public Module
{
public:
	RandomMapGenerator();

	~RandomMapGenerator();

	bool Awake();

	int generateSeed();

	void generateNoise();

	void generateNoise(unsigned int seed);
private:
	time_t msec;
	float frequency;
	float default_seed;
};

#endif // !__RandomMapGenerator__
