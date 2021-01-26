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
	bool passed_mismatch; // indactor of whether an inactive MutS is now bound at the mismatch waiting to be activated
	float p_activate; // the probability of MutS conformational change given that ATP is bound (clamp failure)
	float p_nick; // the probability of a nick being made given that SLH passes the nicking site
	int my_index; // the index indicating how many complexes were bound to the substrate before this complex
	int stepsize;
	NetworkArray network;
	Topology topology;
	XoshiroCpp::Xoshiro128PlusPlus gen;
	std::uniform_real_distribution<> dist;
public:
	float nick1; // time at which site 1 nicked
	float nick2; // time at which site 2 nicked
	float dt_diff;
	float dt_react;
	float totaltime;
	float currenttime;
	int mutS_footprint; // space taken up by a MutS dimer
	std::array<std::vector<float>, 2> dimersactive;	// times at which MutS dimers are activated or inactivated
	std::vector<float> homotetramer; // times at which the complex becomes a homo/hetero-type complex
	std::vector<int> states; // all states the complex has been in
	void assign(NetworkArray &net, ParameterObj &par, XoshiroCpp::Xoshiro128PlusPlus &rng, int my_index, float time, int pos);
	ModelInstance(NetworkArray &net, ParameterObj &par, XoshiroCpp::Xoshiro128PlusPlus &rng, int my_index, float starttime=0.0, int pos=-1);
	ModelInstance();
	int getState() const;
	int getPosition() const;
	void setStep(std::vector<int> &positions);

	// Returns true if going to newposition is possible, false else
	bool stepPossible(std::vector<int> &positions, int newposition, int edge) const;

	// Decides which transition will be followed, if any
	void transition(std::vector<int> &positions);

	// Activates one of the MutS dimers
	void activateS();

	// Checks whether MutSLH is at a nicking site and this site is not nicked yet, and if so stores currenttime in corresponding field
	void nicking(int substrate_id);

	// Updates the stepsize based on diffusion constant in this state
	void updateStep();

	void reactionStep(int timeindex, std::vector<int> &positions);
	void main(std::vector<int> &positions);
};
#endif //MARKOVNET_MODELINSTANCE_H
