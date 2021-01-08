//
// Created by Celly on 28/10/2020.
//

#include "IO.h"

ParameterObj IO::read(int argc, char **arg) {
	ParameterObj parameters;
	if (argc == 1) {
		return parameters;
	}

	if (argc != 8){
		std::cerr << "usage: " << arg[0] << " Sconc Lconc Hconc topology substrate multipleloading" << std::endl;
	}
	parameters.S_conc = std::stof(arg[1])*1e-9;
	parameters.L_conc = std::stof(arg[2])*1e-9;
	parameters.H_conc = std::stof(arg[3])*1e-9;

	std::map<std::string, Topology> str_enum_top {
			{"circular", circular}, {"linear", linear}, {"endblocked", endblocked},
			{"Circular", circular}, {"Linear", linear}, {"Endblocked", endblocked}
	};
	parameters.top = str_enum_top[arg[4]];

	std::map<std::string, Substrate_type> str_enum_subs {
			{"GT1A", GT1A}, {"GT2A", GT2A}, {"GT2B", GT2B}, {"AT1A", AT1A}
	};
	parameters.subs = str_enum_subs[arg[5]];
	parameters.multipleloading = std::string(arg[6]).std::string::compare("True") == 0;

	if (std::string(arg[6]).std::string::compare("Tetramer") == 0){
		parameters.tetramer = true;
	}
	else {
		parameters.tetramer = false;
	}
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

void IO::momentsNicking(std::vector<Substrate *> sims, std::string filename) {
	std::ofstream outstream;
	outstream.open(filename, std::ios::app);
	for (Substrate* model:sims) {
		outstream << model->nick1 << "\t" << model->nick2 << std::endl;
	}
	outstream.close();
}

void IO::momentsActive(std::vector<ModelInstance*> sims, std::string filename){
	std::ofstream outstream;
	outstream.open(filename, std::ios::app);
	for (ModelInstance* model: sims){
		for (auto i: model->dimersactive.at(0)){
			outstream << i << "\t";
		}
		outstream << std::endl;
		for (auto i: model->dimersactive.at(1)){
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

void IO::singleStates(ModelInstance *model, std::string filename) {
	std::ofstream outstream;
	outstream.open(filename, std::ios::app);
	for (int thisstate : model->states){
		outstream << thisstate << "\t";
	}
	outstream << std::endl;
	outstream.close();
}

void IO::writeTransitions(NetworkArray net){
	std::ofstream outstream;
	outstream.open("transitions.tsv");
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
	std::ofstream outstream;
	outstream.open(filename, std::ios::app);
	for (auto timestep: homotetramer_arr){
		outstream << timestep.at(0)/(timestep.at(0)+timestep.at(1)) << "\t"
						 << timestep.at(1)/(timestep.at(0)+timestep.at(1)) << std::endl;
	}
	outstream.close();
}

void IO::writeStates(std::vector<std::array<int, 36>> states_arr, std::string filename) {
	std::ofstream outstream;
	outstream.open(filename, std::ios::app);
	for (auto & timestep: states_arr){
		for (int state: timestep){
			outstream << state << "\t";
		}
		outstream << std::endl;
	}
	outstream.close();
}

void IO::writeAllPositions(std::vector<std::vector<std::vector<int>>> &allpos_arr, std::string filename, int timesteps) {
	std::ofstream outstream;
	outstream.open(filename, std::ios::app);
	for (int i=0; i<timesteps-1; i++) {
		for (auto &substrate : allpos_arr) {
			for (auto &complex : substrate.at(i)) {
				outstream << complex << "\t";
			}
		}
		outstream << std::endl;
	}
}