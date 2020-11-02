//
// Created by Celly on 28/10/2020.
//

#include "IO.h"

ParameterObj IO::read(int argc, char **arg) {
	ParameterObj parameters;
	parameters.S_conc = std::stof(arg[1]);
	parameters.L_conc = std::stof(arg[2]);
	parameters.H_conc = std::stof(arg[3]);
	//ToDo: string to enum for topology, substrate
}

void IO::write(ModelInstance model, std::string filename) {
	std::ofstream output;
	output.open(filename);
	output << model.nick1 << "\t" << model.nick2 << std::endl;
	output.close();
}