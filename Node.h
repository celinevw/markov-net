//
// Created by Celly on 07/10/2020.
//

#ifndef MARKOVNET_NODE_H
#define MARKOVNET_NODE_H

#include "ComplexState.h"
#include <string>
#include <list>

class Node{
private:
	ComplexState complexA;	// state of complex A
	ComplexState complexB;	// state of complex B
	double D;				// diffusion constant
public:
	Node(ComplexState A, ComplexState B);
	Node();
	double getNodeD();
	ComplexState getStateA();
	ComplexState getStateB();

	friend bool operator==(Node n1, Node n2);
};

#endif //MARKOVNET_NODE_H
