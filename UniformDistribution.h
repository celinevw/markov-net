//
// Created by Celly on 21/10/2020.
//

#ifndef MARKOVNET_UNIFORMDISTRIBUTION_H
#define MARKOVNET_UNIFORMDISTRIBUTION_H

#include <iostream>
#include <fstream>
#include <random>
#include <cmath>

class UniformDistribution{
private:
	float low;
	float high;
	std::random_device rd;
	std::mt19937 gen; //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> distrib;
public:
	UniformDistribution(float l, float h);
	float getRandomNumber();
	float getLow();
	float getHigh();
};
#endif //MARKOVNET_UNIFORMDISTRIBUTION_H
