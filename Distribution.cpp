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

ExponentialDistribution::ExponentialDistribution() : Distribution(){
	lambda = 1;
}

/* Smirnov transform to get an exponential distribution
 */
float ExponentialDistribution::getRandomNumber() {
	float u = distrib(gen);
	float x = -1 / lambda * log(1 - u);
	return x;
}

float ExponentialDistribution::getLambda() {
	return lambda;
}

NormalDistribution::NormalDistribution(float m, float s) : Distribution(){
	mu = m;
	sigma = s;
}

NormalDistribution::NormalDistribution() : Distribution(){
	mu = 0;
	sigma = 1;
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

float NormalDistribution::getMu() {
	return mu;
}

float NormalDistribution::getSigma() {
	return sigma;
}

UniformDistribution::UniformDistribution(float l, float h) : Distribution() {
	low = l;
	high = h;
}

UniformDistribution::UniformDistribution() : Distribution() {
	low = 0;
	high = 1;
}

float UniformDistribution::getRandomNumber() {
	return distrib(gen);
}

float UniformDistribution::getLow() {
	return low;
}

float UniformDistribution::getHigh() {
	return high;
}