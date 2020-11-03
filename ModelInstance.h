//
// Created by Celly on 20/10/2020.
//

#ifndef MARKOVNET_MODELINSTANCE_H
#define MARKOVNET_MODELINSTANCE_H

#include "NetworkArray.h"
#include "ParameterObj.h"
#include <random>
#include <vector>

//ToDo: use diffusion coefficient or directly calculate stepsize when entering new state

class ModelInstance{
private:
	int state;
	int position;
	int stepsize;
	float dt;
	float currenttime;
	NetworkArray network;
	Topology topology;
public:
	float nick1;	// time at which site 1 nicked
	float nick2;	// time at which site 2 nicked
	void assign(NetworkArray net, ParameterObj par);
	ModelInstance(NetworkArray net, ParameterObj par);
	ModelInstance();
	int getState();
	int getPosition();
	void setStep(float x);
	void transition(float x);
	void nicking();
	void main();
};
#endif //MARKOVNET_MODELINSTANCE_H
