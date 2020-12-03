//
// Created by Celly on 20/10/2020.
//

#ifndef MARKOVNET_MODELINSTANCE_H
#define MARKOVNET_MODELINSTANCE_H

#include "NetworkArray.h"
#include "ParameterObj.h"
#include "XoshiroCpp.hpp"
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
	XoshiroCpp::Xoshiro128PlusPlus gen;
	std::uniform_real_distribution<> dist;
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
	void assign(NetworkArray &net, ParameterObj &par, XoshiroCpp::Xoshiro128PlusPlus &rng, float time);
	ModelInstance(NetworkArray &net, ParameterObj &par, XoshiroCpp::Xoshiro128PlusPlus &rng, float starttime=0.0);
	ModelInstance();
	int getState();
	int getPosition();
	void setStep(int index, std::vector<int> &positions);
	bool stepPossible(std::vector<int> &positions, int newposition, int index);
	void transition();
	void activateS();
	void nicking();
	void updateStep();
	void reactionStep(int timeindex, int stepsperreaction);
	std::vector<int> main(std::vector<int> &positions);
};
#endif //MARKOVNET_MODELINSTANCE_H
