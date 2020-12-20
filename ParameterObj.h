//
// Created by Celly on 02/11/2020.
//

#ifndef MARKOVNET_PARAMETEROBJ_H
#define MARKOVNET_PARAMETEROBJ_H

#include <string>

// Enum indicating the type of substrate
enum Substrate_type {GT1A, GT2A, GT2B};

// Enum indicating the substrate topology
enum Topology {circular, linear, endblocked};

struct ParameterObj {
	float S_conc = 100e-9;
	float L_conc = 100e-9;
	float H_conc = 100e-9;
	Topology top = endblocked;
	Substrate_type subs = GT2A;
};

#endif //MARKOVNET_PARAMETEROBJ_H
