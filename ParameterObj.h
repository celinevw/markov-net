//
// Created by Celly on 02/11/2020.
//

#ifndef MARKOVNET_PARAMETEROBJ_H
#define MARKOVNET_PARAMETEROBJ_H

#include <string>

enum substrate {GT1A, GT2A, GT2B};
enum topology {circular, linear, endblocked};

struct ParameterObj{
	float S_conc;
	float L_conc;
	float H_conc;
	enum topology top;
	enum substrate subs;
	ParameterObj();
};

#endif //MARKOVNET_PARAMETEROBJ_H
