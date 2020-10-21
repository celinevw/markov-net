//
// Created by Celly on 20/10/2020.
//

#include "ModelInstance.h"

ModelInstance::ModelInstance(Network net, int len, float x) {
	this->position = 10;
	this->length = len;
	this->network = net;

	if (x < 0){
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

void ModelInstance::setStep(float x) {
	//TODO: find step size per type of complex
	int stepsize = 2;

	// Choose direction
	int direction;
	if(x<0){
		direction = - 1;
	}
	else{
		direction = 1;
	}

	// If not stepping off DNA, add step to position
	if(position + direction * stepsize >= 0 && position + direction * stepsize < length){
		position += direction * stepsize;
	}

}

void ModelInstance::transition(float x) {
	std::cout << x << std::endl;

	// No outgoing edges, then stay in this state
	if (network.getEdgesOfNode(state).empty()){
		return;
	}

	// Iterate over outgoing edges, find the one to take depending on the random number x
	float threshold = 0;
	for (Edge e:network.getEdgesOfNode(state)){
		threshold += e.getP();
		if (x < threshold) {
			this->state = e.getEndNode();
			break;
		}
	}

}

