//
// Created by Celly on 09/10/2020.
//

#include "Distribution.h"

Distribution::Distribution() {
	std::random_device rd;		//Will be used to obtain a seed for the random number engine
	gen = std::mt19937 (rd());	//Standard mersenne_twister_engine seeded with rd()
	distrib = std::uniform_real_distribution<> (0,1);
}

ExponentialDistribution::ExponentialDistribution(float l) : Distribution(){
	lambda = l;
}

/* Smirnov transform to get an exponential distribution
 */
float ExponentialDistribution::getRandomNumber() {
	float u = distrib(gen);
	float x = -1 / lambda * log(1 - u);
	return x;
}

NormalDistribution::NormalDistribution(float m, float s) : Distribution(){
	mu = m;
	sigma = s;
}

float NormalDistribution::getRandomNumber() {
	float u1;
	float u2;
	float u3;
	bool accepted = false;

	while(!accepted){
		u1 = this->distrib(gen);
		u2 = this->distrib(gen);
		if (u2 <= exp(-0.5*pow(-log(u1)-1,2))){
			accepted = true;
		}
	}

	u3 = this->distrib(gen);
	if (u3 < 0.5) return sigma*log(u1)+mu;
	else return -sigma*log(u1)+mu;
}

UniformDistribution::UniformDistribution(float l, float h) : Distribution() {
	low = l;
	high = h;
}

float UniformDistribution::getRandomNumber() {
	return distrib(gen);
}