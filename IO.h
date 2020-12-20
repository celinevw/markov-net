//
// Created by Celly on 28/10/2020.
//

#ifndef MARKOVNET_IO_H
#define MARKOVNET_IO_H

#include <vector>
#include <fstream>
#include <iostream>
#include <map>
#include "Substrate.h"

class IO{
public:
	// Reads the given parameters into a Parameter Obj. If no parameters are supplied, returns default ParameterObj.
	ParameterObj read(int argc, char** arg);

	// Takes a vector of Substrates sims and writes the times at which nicks are made to a file with name filename
	void momentsNicking(std::vector<Substrate*> sims, std::string filename);

	// Takes a vector of ModelInstances sims and writes the times at which nicks are made to a file with name filename
	void momentsNicking(std::vector<ModelInstance*> sims, std::string filename);

	// Takes a vector of ModelInstances sims and writes the times at which the complexes
	// are activated or inactivated to a file with name filename
	void momentsActive(std::vector<ModelInstance*> sims, std::string filename);

	// Takes a vector of ModelInstances sims and writes the times at which the complexes
	// become homo/hetero-type complex to a file with name filename
	void momentsHomotetramer(std::vector<ModelInstance*> sims, std::string filename);

	// Takes a ModelInstances model and writes the states that model was in to a file with name filename
	void singleStates(ModelInstance *model, std::string filename);

	// Takes a NetworkArray net and writes the transitions array of net to a file with name "transitions.tsv"
	void writeTransitions(NetworkArray net);

	// Takes a vector of arrays containing fractions of nicking products over time
	// and writes them to a file with name filename
	void writeNicking(std::vector<std::array<float,4>> nicking_arr, std::string filename);

	// Takes a vector of arrays containing the number of active and inactive complexes over time
	// and writes them to a file with name filename
	void writeDimerActivating(std::vector<std::array<int, 4>> dimer_arr, std::string filename);

	// Takes a vector of arrays containing the fraction of homo/hetero-type complexes over time
	// and writes them to a file with name filename
	void writeHomotetramers(std::vector<std::array<float, 2>>homotetramer_arr, std::string filename);

	// Takes a vector of arrays containing the number of complexes in a certain state over time
	// and writes them to a file with name filename
	void writeStates(std::vector<std::array<int, 36>> states_arr, std::string filename);

	// Takes a vector containing positions of all complexes on a number of complexes and the number of timesteps
	// and writes them to a file witn name filename
	void writeAllPositions(std::vector<std::vector<std::vector<int>>> &allpos_arr, std::string filename, int timesteps);
};

#endif //MARKOVNET_IO_H
