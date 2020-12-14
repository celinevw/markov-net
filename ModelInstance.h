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
	bool passed_mismatch;
	float p_activate;
	int my_index;
	int stepsize;
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
	int mutS_footprint;
	std::array<std::vector<float>, 2> dimersactive;	//when MutS dimers are activated
	std::vector<float> homotetramer;	// when complex is a homotetramer
	std::vector<int> states;
	void assign(NetworkArray &net, ParameterObj &par, XoshiroCpp::Xoshiro128PlusPlus &rng, int my_index, float time, int pos);
	ModelInstance(NetworkArray &net, ParameterObj &par, XoshiroCpp::Xoshiro128PlusPlus &rng, int my_index, float starttime=0.0, int pos=-1);
	ModelInstance();
	int getState() const;
	int getPosition() const;
	void setStep(std::vector<int> &positions);
	bool stepPossible(std::vector<int> &positions, int newposition, int edge) const;
	void transition(std::vector<int> &positions);
	void activateS();
	void nicking();
	void updateStep();
	void reactionStep(int timeindex, std::vector<int> &positions);
	void main(std::vector<int> &positions);
};
#endif //MARKOVNET_MODELINSTANCE_H
