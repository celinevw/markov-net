//
// Created by Celly on 20/10/2020.
//

#include "ModelInstance.h"

ModelInstance::ModelInstance(NetworkArray net, float x) {
	position = net.mismatchsite; // starting position and mismatch position must be the same
	network = net;
	stepsize = 1;

	if (x < 0.5){
		state = 1;
	}
	else {
		state = 6;
	}
	diffusioncoefficient = net.diffusion.at(state);
}

ModelInstance::ModelInstance() {
	ModelInstance(NetworkArray(),0.0);
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
	//TODO: find step size per type of complex

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
			if (!attachingSi){ // if not adding Si, position does not matter
				state = index;
			}
			// else make sure it is close enough or do nothing
			else if(std::abs(position - network.mismatchsite) < 2 * stepsize){
				state = index;
			}
			break;
		}
		index++;
	}
}

