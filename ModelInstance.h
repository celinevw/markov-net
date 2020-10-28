//
// Created by Celly on 20/10/2020.
//

#ifndef MARKOVNET_MODELINSTANCE_H
#define MARKOVNET_MODELINSTANCE_H

#include "NetworkArray.h"
#include <random>
#include <vector>

class ModelInstance{
private:
	int state;
	int position;
	float diffusioncoefficient;
	int stepsize;
	NetworkArray network;
	int nick1;	// number of times site 1 nicked
	int nick2;	// number of times site 2 nicked
public:
	ModelInstance(NetworkArray net, float x);
	ModelInstance();
	int getState();
	int getPosition();
	void setStep(float x);
	void transition(float x1);
	void main();
};
#endif //MARKOVNET_MODELINSTANCE_H
