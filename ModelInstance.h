//
// Created by Celly on 20/10/2020.
//

#ifndef MARKOVNET_MODELINSTANCE_H
#define MARKOVNET_MODELINSTANCE_H

#include "Network.h"
#include <random>

class ModelInstance{
private:
	int state;
	int length;
	int position;
	int mismatch;
	int stepsize;
	int nickingsite;
	Network network;
public:
	ModelInstance(Network net, int len, float x);
	int getState();
	int getPosition();
	void setStep(float x);
	void transition(float x1);
};
#endif //MARKOVNET_MODELINSTANCE_H
