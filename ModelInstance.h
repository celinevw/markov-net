//
// Created by Celly on 20/10/2020.
//

#ifndef MARKOVNET_MODELINSTANCE_H
#define MARKOVNET_MODELINSTANCE_H

#include "NetworkArray.h"
#include "ParameterObj.h"
#include <random>
#include <vector>
#include <cmath>

class ModelInstance{
private:
	int state;
	int position;
	int stepsize;
	float dt;
	float dt_diff;
	float currenttime;
	NetworkArray network;
	Topology topology;
public:
	float nick1;	// time at which site 1 nicked
	float nick2;	// time at which site 2 nicked
	std::vector<float> firstbound; 		// when first dimer is activated or falls off
	std::vector<float> secondbound;		// when second dimer is activated or falls off
	void assign(NetworkArray net, ParameterObj par);
	ModelInstance(NetworkArray net, ParameterObj par);
	ModelInstance();
	int getState();
	int getPosition();
	void setStep(float x);
	void transition(float x);
	void nicking(float x);
	void updateStep();
	void main(std::vector<float> *numbers_ptr);
};
#endif //MARKOVNET_MODELINSTANCE_H
