//
// Created by Celly on 20/10/2020.
//

#include "ModelInstance.h"

void ModelInstance::assign(NetworkArray net, ParameterObj par) {
	network = net;
	position = network.mismatchsite;	// starting position and mismatch position must be the same
	state = 1;						// graph is symmetric, so let all start in state 1
	dt = 0.5; 						// for reaction, check dt with networkarray for probabilities
	dt_diff = 100e-6;				// for diffusion
	updateStep();
	nick1 = -1;
	nick2 = -1;
	currenttime = 0;
	topology = par.top;
}

ModelInstance::ModelInstance(NetworkArray net, ParameterObj par) {
	assign(net, par);
}

ModelInstance::ModelInstance() {
	NetworkArray myNet;
	ParameterObj myPars;
	assign(myNet, myPars);
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
void ModelInstance::setStep(float x) {
	// Choose direction
	int direction;
	if(x<0.5){
		direction = -1;
	}
	else{
		direction = 1;
	}

	// If not stepping off DNA, add step to position
	if(position + direction * stepsize >= 0 && position + direction * stepsize < network.length){
		position += (direction * stepsize);
	}
	else if (topology == circular){
		// go to the "other side"
		position = (position + direction * stepsize) % network.length;
	}
	else if (topology == linear){
		// fall off, go to none-none state
		// ToDo: fall off completely or only one of the dimers?
		state = 0;
		updateStep();
	}
	// if endblocked, do not take a step.

}

/* Choose transition to follow based on random number x.
 */
void ModelInstance::transition(float x) {
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
			//It is not one of transitions where Si binds the mismatch
			bool attachingSi = (state < 6 && index == state + 6) ||
							   (state % 6 == 0 && index == state + 1);
			if (!attachingSi || std::abs(position - network.mismatchsite) < 2 * stepsize){
				// if not adding Si, position does not matter, else make sure it is close enough or do nothing
				state = index;
				updateStep();
			}
			break;
		}
		index++;
	}
}

void ModelInstance::nicking(float x){
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

/* main method, one run of a model instance
 */
void ModelInstance::main(std::vector<float> *numbers_ptr) {
	int stepsperreaction = roundf(dt / dt_diff);

	auto it = numbers_ptr->begin();
	int i=0;

	while (currenttime <= 600 && (nick1<0 || nick2<0)) {
		currenttime = dt_diff * i; //update only needed when time may be used
		setStep(*(it++));
		nicking(*(it++));

		if(i % stepsperreaction == 0) {
			transition(*(it++));
		}
		i++;
	}
}
