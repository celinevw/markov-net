//
// Created by Celly on 20/10/2020.
//

#include "ModelInstance.h"

void ModelInstance::assign(NetworkArray &net, ParameterObj &par, XoshiroCpp::Xoshiro128PlusPlus &rng, float time) {
	network = net;
	position = network.mismatchsite;	// starting position and mismatch position must be the same
	state = 1;						// graph is symmetric, so let all start in state 1
	dt_react = net.dt_react; 						// for reaction, check dt with networkarray for probabilities
	dt_diff = 100e-6;				// for diffusion
	totaltime = 5;
	updateStep();
	nick1 = -1;
	nick2 = -1;
	currenttime = time;
	passed_mismatch = false;
	p_activate = 1;
	topology = par.top;

	dimersactive = std::array<std::vector<float>, 2> {};
	homotetramer = std::vector<float>{};
	states = std::vector<int>(totaltime/dt_react, 0);
	gen = rng;
	dist = std::uniform_real_distribution<> (0,1);
}

ModelInstance::ModelInstance(NetworkArray &net, ParameterObj &par, XoshiroCpp::Xoshiro128PlusPlus &rng, float start) {
	assign(net, par, rng, start);
}

ModelInstance::ModelInstance() {
	NetworkArray myNet;
	ParameterObj myPars;
	XoshiroCpp::Xoshiro128PlusPlus rng (1000);
	assign(myNet, myPars, rng, 0.0);
}

int ModelInstance::getState() {
	return this->state;
}

int ModelInstance::getPosition() {
	return position;
}

/* Randomly decide direction. Check whether new position lies within boundaries
 * If so, update step, else don't.
 */
void ModelInstance::setStep(int my_index, std::vector<int> &positions) {
	float x = dist(gen);
	// Choose direction
	int direction = 0;
	int newposition = position;
	if(x<0.5){
		direction = -1;
	}
	else{
		direction = 1;
	}

	// If not stepping off DNA, add step to position
	if(position + direction * stepsize >= 0 && position + direction * stepsize < network.length){
		newposition = position + (direction * stepsize);
	}
	else if (topology == circular){
		// go to the "other side"
		newposition = (position + direction * stepsize + network.length) % network.length;
	}
	else if (topology == linear){
		// fall off, go to none-none state
		state = 0;
		newposition = -1;
		updateStep();
	}
	// if endblocked, do not take a step.

	if(newposition != position && stepPossible(positions, newposition, my_index)) {
		position = newposition;
		positions.at(my_index) = position;

	}

	passed_mismatch = (state / 6 == 1 || state % 6 == 1) &&
					  (std::abs((position - network.nickingsite1)) < stepsize
					   || std::abs((position - network.nickingsite2)) < stepsize);
}

bool ModelInstance::stepPossible(std::vector<int> &positions, int newposition, int my_index) {
	for (int i = 0; i< positions.size(); i++) {
		if (newposition == positions.at(i) && i != my_index){
			return false;
		}
	}
	return true;
}

/* Choose transition to follow based on random number x.
 */
void ModelInstance::transition() {
	float x = dist(gen);
	// No outgoing edges, then stay in this state
	if (std::accumulate(network.transitions.at(state).begin(), network.transitions.at(state).end(), 0.0) == 0){
		return;
	}

	// Iterate over outgoing edges, find the one to take depending on the random number x
	std::array<float, 36> cumulative{};
	std::partial_sum(network.transitions.at(state).begin(), network.transitions.at(state).end(), cumulative.begin());
	int index = 0;
	for (float threshold : cumulative){
		if (x < threshold){
			//It is not one of transitions where S is activated
			bool activatingS = (state / 6 == 1 && index == state + 6) ||
							   (state % 6 == 1 && index == state + 1);
			if (!activatingS){
				if (state % 6 >= 2 && index == state - (state % 6)) {
					dimersactive.at(0).push_back(currenttime);
					//std::cout << "inactivate complex 1" << std::endl;
				}
				if (state / 6 >= 2 && index == state % 6) {
					dimersactive.at(1).push_back(currenttime);
					//std::cout << "inactivate complex 2" << std::endl;
				}
				// if not adding Si, position does not matter, else make sure it is close enough or do nothing
				state = index;
				updateStep();
			}
			break;
		}
		index++;
	}
}

void ModelInstance::activateS(){
	float x = dist(gen);
	if (x >= p_activate){
		return;
	}
	if ((state / 6 != 1) || (state % 6 == 1 && x < (p_activate / 2))) {	// if only complex 1 can activate, or choose randomly between the two
		state = state + 1;
		dimersactive.at(0).push_back(currenttime);
		//std::cout << "activate complex 1" << std::endl;
	}
	else {										// if only complex 2 can activate, or choose randomly
		state = state + 6;
		dimersactive.at(1).push_back(currenttime);
		//std::cout << "activate complex 2" << std::endl;
	}
}

void ModelInstance::nicking(){
	float x = dist(gen);
	if (state > 29 || state % 6 == 5){ // if one complex is SLH, possible nicking if not nicked yet
		float p_nick = 1;
		if (std::abs((position - network.nickingsite1)) < stepsize && nick1 < 0 && x < p_nick){
			nick1 = currenttime;
		}
		if (std::abs((position - network.nickingsite2)) < stepsize && nick2 < 0 && x < p_nick){
			nick2 = currenttime;
		}
	}
}

void ModelInstance::updateStep() {
	stepsize = round(std::sqrt(2*network.diffusion.at(state)*dt_diff)*10000/3.4); //micrometer->armstrong->bp
}

void ModelInstance::reactionStep(int timeindex, int stepsperreaction) {
	int oldstate = state;

	states.at(timeindex/stepsperreaction) = state;

	if (passed_mismatch) {
		activateS();
		passed_mismatch = false;
	} else {
		transition();
	}

	if (state / 6 == state % 6 ^ oldstate / 6 == oldstate % 6) {
		homotetramer.push_back(currenttime);
	}
	if (oldstate / 6 == oldstate % 6 && state == 0) {
		homotetramer.push_back(currenttime);
	}
}

/* main method, one run of a model instance
 */
std::vector<int> ModelInstance::main(std::vector<int> &positions) {
	std::cout << currenttime << "\tModelinstance" << std::endl;
	int stepsperreaction = roundf(dt_react / dt_diff);
	int i = currenttime / dt_diff;
	int oldstate;
	std::vector<int> my_pos;

	if (i != 0) {
		my_pos.assign(i, -1);
	}

	while (dt_diff * i < totaltime && (nick1<0 || nick2<0) && state != 0) { //
		currenttime = dt_diff * i; //update only needed when time may be used
		my_pos.push_back(position);
		setStep(i, positions);
		nicking();

		if(i % stepsperreaction == 0) {
			reactionStep(i, stepsperreaction);
		}

		if(i % 10000 == 0){
			int x = 0;
		}

		i++;
	}

	return my_pos;
}
