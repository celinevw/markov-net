//
// Created by Celly on 07/10/2020.
//

#include "Node.h"
#include <cmath>

Node::Node(ComplexState A, ComplexState B) {
	complexA = A;
	complexB = B;

	//only one complex, that one determines diffusion
	if(A.getStateName().std::string::compare("none") == 0){
		this->D = B.getStateD();
	}
	else if (B.getStateName().std::string::compare("none") == 0){
		this->D = A.getStateD();
	}
	// if one complex is Si, stationary at mismatch so no diffusion
	else if(A.getStateName().std::string::compare("Si") == 0 || B.getStateName().std::string::compare("Si") == 0){
		this->D = 0;
	}
	else{
		this->D = pow(pow(A.getStateD(), -1) + pow(B.getStateD(), -1), -1);
	}
}

Node::Node(){
	complexA = ComplexState();
	complexB = ComplexState();
	D = 0;
}

ComplexState Node::getStateA() {
	return this->complexA;
}

ComplexState Node::getStateB() {
	return this->complexB;
}

double Node::getNodeD() {
	return this->D;
}

/* Return true of the nodes have the same two states, in the same order.
 */
bool operator==(Node n1, Node n2){
	return (n1.getStateA() == n2.getStateA() && n1.getStateB() == n2.getStateB());
}