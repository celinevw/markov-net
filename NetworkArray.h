//
// Created by Celly on 23/10/2020.
//

#ifndef MARKOVNET_NETWORKARRAY_H
#define MARKOVNET_NETWORKARRAY_H

#include <array>
#include <cmath>
#include "ParameterObj.h"

float RateToProbability(float x, float dt);

class NetworkArray{
public:
	void assign(ParameterObj par);
	NetworkArray(ParameterObj par);
	NetworkArray();
	std::array<std::array<float, 36>, 36> transitions;
	std::array<float, 36> diffusion;
	int length;			//bp
	int mismatchsite;
	int nickingsite1;
	int nickingsite2;
};

#endif //MARKOVNET_NETWORKARRAY_H
