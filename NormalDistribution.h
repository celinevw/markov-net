//
// Created by Celly on 07/10/2020.
//

#ifndef MARKOVNET_NORMALDISTRIBUTION_H
#define MARKOVNET_NORMALDISTRIBUTION_H

#include <iostream>
#include <fstream>
#include <random>
#include <cmath>

class NormalDistribution{
private:
	float mu;
	float sigma;
	std::random_device rd;
	std::mt19937 gen; //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> distrib;
public:
	NormalDistribution(float m, float s);
	float getRandomNumber();
	float getMu();
	float getSigma();
};

#endif //MARKOVNET_NORMALDISTRIBUTION_H
