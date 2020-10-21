//
// Created by Celly on 21/10/2020.
//

#include "TestingDistributions.h"

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


void testNormalDist(){
	std::ofstream outfile;
	outfile.open ("normal.txt");
	int num = 2500;
	float mu = 2;
	float sigma = 5;
	NormalDistribution myNormal(mu, sigma);
	outfile << mu << std::endl << sigma << std::endl << num << std::endl;

#pragma omp parallel
	{
#pragma omp for
		for(size_t i=0; i<num; i++) {
			outfile << myNormal.getRandomNumber() << std::endl;
		}
	}
}