//
// Created by Celly on 28/10/2020.
//

#include "IO.h"

ParameterObj IO::read(int argc, char **arg) {
	ParameterObj parameters;
	if (argc == 1) {
		return parameters;
	}

	if (argc != 6){
		std::cerr << "usage: " << arg[0] << " Sconc Lconc Hconc topology substrate" << std::endl;
	}
	parameters.S_conc = std::stof(arg[1]);
	parameters.L_conc = std::stof(arg[2]);
	parameters.H_conc = std::stof(arg[3]);

	std::map<std::string, Topology> str_enum_top {
			{"circular", circular}, {"linear", linear}, {"endblocked", endblocked}
	};
	parameters.top = str_enum_top[arg[4]];

	std::map<std::string, Substrate> str_enum_subs {
			{"GT1A", GT1A}, {"GT2A", GT2A}, {"GT2B", GT2B}
	};
	parameters.subs = str_enum_subs[arg[5]];
	return parameters;
}

void IO::write(ModelInstance model, std::string filename) {
	std::ofstream output;
	output.open(filename, std::ios::app);
	output << model.nick1 << "\t" << model.nick2 << std::endl;
	output.close();
}

void IO::writeTransitions(NetworkArray net){
	std::ofstream output;
	output.open("transitions.tsv");
	for(auto state_from: net.transitions){
		for (auto state_to: state_from){
			output << state_to << "\t";
		}
		output << std::endl;
	}
	output.close();
}