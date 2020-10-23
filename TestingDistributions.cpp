//
// Created by Celly on 21/10/2020.
//

#include "TestingDistributions.h"

// CCC Please, please, please, make every test return type bool, where it denotes if the test is passed. Don't test stuff by eye, there's statistics to help you. Even simple stats, like the closed-form expression of the first 2 moments is often enough to spot major errors

void testExpDist(){
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
			outfile << myExp.getRandomNumber() << std::endl; // CCC You are mutex-locked here. Get num different instances of myExp and it should work.
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

void testUniformDist(){
	std::ofstream outfile;
	outfile.open ("uniform.txt");
	int num = 2500;
	float l = 0;
	float h = 1;
	UniformDistribution myUniform(l, h);
	outfile << l << std::endl << h << std::endl << num << std::endl;

#pragma omp parallel
	{
#pragma omp for
		for(size_t i=0; i<num; i++) {
			outfile << myUniform.getRandomNumber() << std::endl;
		}
	}
}
