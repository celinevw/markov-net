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
	//TODO: find step size per type of complex;
	int direction;
	int stepsize = 2;
	if(x<0){
		direction = - 1;
	}
	else{
		direction = 1;
	}

	if(position + direction * stepsize >= 0 && position + direction * stepsize < length){
		position += direction;
	}

}

void ModelInstance::transition(float x1, float x2) {
	std::cout << x1 << " " << x2 << std::endl;

	if (network.getEdgesOfNode(state).empty()){
		return;
	}

	float threshold = 0;
	for (Edge e:network.getEdgesOfNode(state)){
		threshold += e.getP();
		if (x1< threshold) {
			Node newstate = e.getEndNode(); //TODO: find problem in std::find newstate
			auto it1 = std::find(network.getNodes().begin(), network.getNodes().end(), newstate);
			if (it1 != network.getNodes().end()) {
				int stateindex = std::distance(network.getNodes().begin(), it1);
			}
			break;
		}
	}

}

