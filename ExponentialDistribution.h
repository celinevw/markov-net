//
// Created by Celly on 09/10/2020.
//

#ifndef MARKOVNET_EXPONENTIALDISTRIBUTION_H
#define MARKOVNET_EXPONENTIALDISTRIBUTION_H

#include <random>
#include <fstream>

class ExponentialDistribution{
private:
	float lambda;
	std::random_device rd;
	std::mt19937 gen; //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> distrib;
public:
	ExponentialDistribution(float l);
	float getLambda();
	float getRandomNumber();
	static void testExpDist();
};

#endif //MARKOVNET_EXPONENTIALDISTRIBUTION_H
