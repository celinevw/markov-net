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
#include <iostream>

class ModelInstance{
private:
	int state;
	int position;
	int stepsize;
	bool passed_mismatch;
	float p_activate;
	NetworkArray network;
	Topology topology;
public:
	float nick1;	// time at which site 1 nicked
	float nick2;	// time at which site 2 nicked
	float dt_diff;
	float dt_react;
	float totaltime;
	float currenttime;
	std::array<std::vector<float>, 2> dimersactive;	//when MutS dimers are activated
	std::vector<float> homotetramer;	// when complex is a homotetramer
	std::vector<int> states;
	void assign(NetworkArray net, ParameterObj par);
	ModelInstance(NetworkArray net, ParameterObj par);
	ModelInstance();
	int getState();
	int getPosition();
	void setStep(float x);
	void transition(float x);
	void activateS(float x);
	void nicking(float x);
	void updateStep();
	void main(std::vector<float> *numbers_ptr);
};
#endif //MARKOVNET_MODELINSTANCE_H
