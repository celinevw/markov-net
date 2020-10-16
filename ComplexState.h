//
// Created by Celly on 07/10/2020.
//

#ifndef MARKOVNET_COMPLEXSTATE_H
#define MARKOVNET_COMPLEXSTATE_H

#include <string>

class ComplexState{
private:
	std::string name;	// name of complex
	double D;			// diffusion constant of single complex
public:
	ComplexState(std::string nm, double diff);
	ComplexState();
	double getStateD();
	std::string getStateName();
	friend bool operator==(ComplexState s1, ComplexState s2);
};

#endif //MARKOVNET_COMPLEXSTATE_H
