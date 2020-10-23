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
	virtual float getRandomNumber();
};

class ExponentialDistribution: public Distribution{
private:
	float lambda;
public:
	ExponentialDistribution(float l);
	float getRandomNumber() override;
};

class NormalDistribution: public Distribution{
private:
	float mu;
	float sigma;
public:
	NormalDistribution(float m, float s);
	float getRandomNumber() override;
};

class UniformDistribution: public Distribution{
private:
	float low;
	float high;
public:
	UniformDistribution(float l, float h);
	float getRandomNumber() override;
};

#endif //MARKOVNET_DISTRIBUTION_H
