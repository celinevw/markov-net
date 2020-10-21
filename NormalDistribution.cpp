//
// Created by Celly on 07/10/2020.
//

#include "NormalDistribution.h"

NormalDistribution::NormalDistribution(float m, float s) {
	mu = m;
	sigma = s;
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	gen = std::mt19937 (rd()); //Standard mersenne_twister_engine seeded with rd()
	distrib = std::uniform_real_distribution<> (0,1);
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
