//
// Created by Celly on 07/10/2020.
//

#include "ComplexState.h"

ComplexState::ComplexState(std::string nm, double diff) {
	name = nm;
	D = diff;
}

ComplexState::ComplexState() {
	name = "empty";
	D = 0;
}

double ComplexState::getStateD(){
	return D;
}

std::string ComplexState::getStateName(){
	return name;
}

bool operator==(ComplexState s1, ComplexState s2){
	return(s1.getStateName()==s2.getStateName() && s1.getStateD()==s2.getStateD());
}