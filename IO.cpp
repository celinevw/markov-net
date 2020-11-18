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

void IO::momentsNicking(std::vector<ModelInstance*> sims, std::string filename) {
	std::ofstream outstream;
	outstream.open(filename, std::ios::app);
	for (ModelInstance* model: sims){
		outstream << model->nick1 << "\t" << model->nick2 << std::endl;
	}
	outstream.close();
}

void IO::momentsActive(std::vector<ModelInstance*> sims, std::string filename){
	std::ofstream outstream;
	outstream.open(filename, std::ios::app);
	for (ModelInstance* model: sims){
		for (auto i: model->firstbound){
			outstream << i << "\t";
		}
		outstream << std::endl;
		for (auto i: model->secondbound){
			outstream << i << "\t";
		}
		outstream << std::endl;
	}
	outstream.close();
}

void IO::momentsHomotetramer(std::vector<ModelInstance*> sims, std::string filename){
	std::ofstream outstream;
	outstream.open(filename, std::ios::app);
	for (auto model : sims){
		for (auto time : model->homotetramer) {
			outstream << time << "\t";
		}
		outstream << std::endl;
	}
	outstream.close();
}

void IO::writeTransitions(NetworkArray net){
	std::ofstream outstream;
	outstream.open("transitions.tsv", std::ios::app);
	for(auto state_from: net.transitions){
		for (auto state_to: state_from){
			outstream << state_to << "\t";
		}
		outstream << std::endl;
	}
	outstream.close();
}

void IO::writeNicking(std::vector<std::array<float, 4>> nicking_arr, std::string filename) {
	std::ofstream outstream;
	outstream.open(filename, std::ios::app);
	for (auto timestep: nicking_arr){
		for (auto nicked:  timestep){
			outstream << nicked << "\t";
		}
		outstream << std::endl;
	}
	outstream.close();
}

void IO::writeDimerActivating(std::vector<std::array<int, 4>> dimer_arr, std::string filename) {
	std::ofstream outstream;
	outstream.open(filename, std::ios::app);
	for (auto timestep: dimer_arr){
		for (auto bound:  timestep){
			outstream << bound << "\t";
		}
		outstream << std::endl;
	}
	outstream.close();
}

void IO::writeHomotetramers(std::vector<std::array<float, 2>> homotetramer_arr, std::string filename) {
	std::ofstream homotetramer_out;
	homotetramer_out.open(filename, std::ios::app);
	for (auto timestep: homotetramer_arr){
		homotetramer_out << timestep.at(0)/(timestep.at(0)+timestep.at(1)) << "\t"
						 << timestep.at(1)/(timestep.at(0)+timestep.at(1)) << std::endl;
	}
	homotetramer_out.close();
}