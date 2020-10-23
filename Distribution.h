//
// Created by Celly on 09/10/2020.
//

#ifndef MARKOVNET_DISTRIBUTION_H
#define MARKOVNET_DISTRIBUTION_H

#include <random>
#include <fstream>

class Distribution{
protected:
	std::random_device rd;
	std::uniform_real_distribution<> distrib;
	std::mt19937 gen;	//Standard mersenne_twister_engine seeded with rd()
public:
	Distribution();
	virtual float getRandomNumber() = 0;
};

class ExponentialDistribution: public Distribution{
private:
	float lambda;
public:
	ExponentialDistribution(float l);
	ExponentialDistribution();
	float getRandomNumber() override;
	float getLambda();
};

class NormalDistribution: public Distribution{
private:
	float mu;
	float sigma;
public:
	NormalDistribution(float m, float s);
	NormalDistribution();
	float getRandomNumber() override;
	float getMu();
	float getSigma();
};

class UniformDistribution: public Distribution{
private:
	float low;
	float high;
public:
	UniformDistribution(float l, float h);
	UniformDistribution();
	float getRandomNumber() override;
	float getLow();
	float getHigh();
};

#endif //MARKOVNET_DISTRIBUTION_H
