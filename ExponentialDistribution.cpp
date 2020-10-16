//
// Created by Celly on 09/10/2020.
//

#include "ExponentialDistribution.h"

ExponentialDistribution::ExponentialDistribution(float l) {
	this->lambda = l;

	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	gen = std::mt19937 (rd()); //Standard mersenne_twister_engine seeded with rd()
	distrib = std::uniform_real_distribution<> (0,1);
}


float ExponentialDistribution::getLambda() {
	return this->lambda;
}

float ExponentialDistribution::getRandomNumber() {
	float u = distrib(gen);
	float x = -1 / lambda * log(1 - u);
	return x;
}

void ExponentialDistribution::testExpDist(){
	std::ofstream outfile;
	outfile.open ("exponential.txt");
	int num = 2500;
	float lambda = 3;
	ExponentialDistribution myExp(lambda);
	outfile << lambda << std::endl << num << std::endl;

#pragma omp parallel
	{
#pragma omp for
		for(size_t i=0; i<num; i++) {
			outfile << myExp.getRandomNumber() << std::endl;
		}
	}
}