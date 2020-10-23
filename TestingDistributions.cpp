//
// Created by Celly on 21/10/2020.
//

#include "TestingDistributions.h"

// CCC Please, please, please, make every test return type bool, where it denotes if the test is passed. Don't test stuff by eye, there's statistics to help you. Even simple stats, like the closed-form expression of the first 2 moments is often enough to spot major errors

std::tuple<float, float> getMoments (std::vector <float> results){
	float sum = std::accumulate(results.begin(), results.end(), 0.0);
	float mean = sum / results.size();

	float sq_sum = 0.0;
	for (float x: results){
		sq_sum += (x-mean)*(x-mean);
	}
	float variance = sq_sum / results.size();

	return std::make_tuple(mean, variance);
}

bool testDistExp(){
	ExponentialDistribution myExp(3);
	const int num = 2500;
	std::vector<float> results;
	for(size_t i=0; i<num; i++) {
		results.push_back(myExp.getRandomNumber());
	}

	auto moments = getMoments(results);
	float mom1 = 1/myExp.getLambda();
	float mom2 = mom1*mom2;

	if ((std::get<0>(moments) - mom1 )/mom1 < 0.1 && (std::get<1>(moments) - mom2 )/mom2 < 0.1){
		return true;
	}
	else return false;
}


bool testDistNorm() {
	NormalDistribution myNorm(2,1);
	const int num = 2500;
	std::vector<float> results;
	for (size_t i = 0; i < num; i++) {
		results.push_back(myNorm.getRandomNumber());
	}

	auto moments = getMoments(results);
	float mom1 = myNorm.getMu();
	float mom2 = pow(myNorm.getMu(), 2)+ pow(myNorm.getSigma(),2);

	if ((std::get<0>(moments) - mom1 )/mom1 < 0.1 && (std::get<1>(moments) - mom2 )/mom2 < 0.1){
		return true;
	}
	else return false;
}

bool testDistUni(){
	UniformDistribution myUni(3,5);
	const int num = 2500;
	std::vector<float> results;
	for (size_t i = 0; i < num; i++) {
		results.push_back(myUni.getRandomNumber());
	}

	auto moments = getMoments(results);
	float a = myUni.getLow();
	float b = myUni.getHigh();
	float mom1 = (a+b)/2;
	float mom2 = (pow(a,3)-pow(b, 3))/(3*b-3*a);

	if ((std::get<0>(moments) - mom1 )/mom1 < 0.1 && (std::get<1>(moments) - mom2 )/mom2 < 0.1){
		return true;
	}
	else return false;
}
