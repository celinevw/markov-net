//
// Created by Celly on 07/10/2020.
//

#include "Node.h"
#include <cmath>

Node::Node(ComplexState A, ComplexState B) {
	complexA = A;
	complexB = B;
	if(A.getStateName().std::string::compare("none") == 0){
		this->D = B.getStateD();
	}
	else if (B.getStateName().std::string::compare("none") == 0){
		this->D = A.getStateD();
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

bool operator==(Node n1, Node n2){
	return (n1.getStateA() == n2.getStateA() && n1.getStateB() == n2.getStateB() && n1.getNodeD() == n2.getNodeD());
}