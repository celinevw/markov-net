//
// Created by Celly on 02/11/2020.
//

#ifndef MARKOVNET_PARAMETEROBJ_H
#define MARKOVNET_PARAMETEROBJ_H

#include <string>

enum substrate {GT1A, GT2A, GT2B};
enum topology {circular, linear, endblocked};

struct ParameterObj {
	float S_conc = 1e-9;
	float L_conc = 1e-9;
	float H_conc = 1e-9;
	topology top = endblocked;
	substrate subs = GT2A;
};

#endif //MARKOVNET_PARAMETEROBJ_H
