//
// Created by Celly on 20/10/2020.
//

#include "ModelInstance.h"

ModelInstance::ModelInstance(Network net, int len, float x) {
	this->position = 10; // starting position and mismatch position must be the same
	this->mismatch = 10;
	this->length = len;
	this->network = net;
	this->stepsize = 1;
	this->nickingsite = 80;

	if (x < 0.5){
		this->state = 1;
	}
	else {
		this->state = 6;
	}
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
	if(position + direction * stepsize >= 0 && position + direction * stepsize < length){
		position += (direction * stepsize);
	}

}

/* Choose transition to follow based on random number x.
 */
void ModelInstance::transition(float x) {

	// No outgoing edges, then stay in this state
	if (network.getEdgesOfNode(state).empty()){
		return;
	}

	// Iterate over outgoing edges, find the one to take depending on the random number x
	float threshold = 0;
	for (Edge e:network.getEdgesOfNode(state)){
		threshold += e.getP();
		if (x < threshold) {
			//It is not one of transitions where Si binds the mismatch
			bool attachingSi = (this->state < 6 && e.getEndNode() == this->state + 6) ||
					(this->state % 6 == 0 && e.getEndNode() == this->state + 1);
			if (!attachingSi){ // if not adding Si, position does not matter
				this->state = e.getEndNode();
			}
			// else make sure it is close enough or do nothing
			else if(std::abs(this->position - this->mismatch) < 2*this->stepsize){
				this->state = e.getEndNode();
			}
			break;
		}
	}
}

