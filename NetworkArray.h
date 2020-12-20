//
// Created by Celly on 23/10/2020.
//

#ifndef MARKOVNET_NETWORKARRAY_H
#define MARKOVNET_NETWORKARRAY_H

#include <array>
#include <cmath>
#include "ParameterObj.h"

// Converts a conformational change rate to probability of conf. change in dt seconds.
float RateToProbability(float x, float dt);

class NetworkArray{
public:
	void assign(ParameterObj par);
	NetworkArray(ParameterObj par);
	NetworkArray();
	std::array<std::array<float, 36>, 36> transitions; // Matrix containing all transition probabilities
	std::array<float, 36> diffusion; // Array containing all diffusion coefficients
	int length;	//Substrate length in bp
	int mismatchsite; // Position of the mismatchsite
	int nickingsite1; // Position of nicking site 1
	int nickingsite2; // Position of nicking site 2
	float dt_react;
	float activationS; // Probability of activating a mismatch-bound MutS dimer
};

#endif //MARKOVNET_NETWORKARRAY_H
