//
// Created by Celly on 23/10/2020.
//

#ifndef MARKOVNET_NETWORKARRAY_H
#define MARKOVNET_NETWORKARRAY_H

#include <array>
#include <cmath>

float RateToProbability(float x, float dt);

class NetworkArray{
public:
	NetworkArray(int l, int mm, int n1, int n2);
	NetworkArray();
	std::array<std::array<float, 6>, 6> transitions;
	int length;
	int mismatchsite;
	int nickingsite1;
	int nickingsite2;
};

#endif //MARKOVNET_NETWORKARRAY_H
