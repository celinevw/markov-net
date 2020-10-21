//
// Created by Celly on 21/10/2020.
//


#include "UniformDistribution.h"

UniformDistribution::UniformDistribution(float l, float h) {
	low = l;
	high = h;
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	gen = std::mt19937 (rd()); //Standard mersenne_twister_engine seeded with rd()
	distrib = std::uniform_real_distribution<> (l,h);
}

float UniformDistribution::getRandomNumber() {
	return distrib(gen);
}

float UniformDistribution::getLow() {
	return this->low;
}

float UniformDistribution::getHigh(){
	return this->high;
}