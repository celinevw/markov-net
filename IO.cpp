//
// Created by Celly on 28/10/2020.
//

#include "IO.h"

std::vector<float> IO::read(int argc, char **arg) {
	std::vector<float> argvec;
	//Todo: usage message, dep on what arguments can be passed
	for (int i = i; i<argc; i++){
		argvec.push_back(std::atof(arg[i]));
	}
}

void IO::write(ModelInstance model, std::string filename) {
	std::ofstream output;
	output.open(filename);
	output << model.nick1 << "\t" << model.nick2 << std::endl;
	output.close();
}