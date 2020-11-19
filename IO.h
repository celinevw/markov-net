//
// Created by Celly on 28/10/2020.
//

#ifndef MARKOVNET_IO_H
#define MARKOVNET_IO_H

#include <vector>
#include <fstream>
#include <iostream>
#include <map>
#include "ModelInstance.h"
#include "ParameterObj.h"

class IO{
public:
	ParameterObj read(int argc, char** arg);
	void momentsNicking(std::vector<ModelInstance*> sims, std::string filename);
	void momentsActive(std::vector<ModelInstance*> sims, std::string filename);
	void momentsHomotetramer(std::vector<ModelInstance*> sims, std::string filename);
	void singleStates(ModelInstance *model, std::string filename);
	void writeTransitions(NetworkArray net);
	void writeNicking(std::vector<std::array<float,4>> nicking_arr, std::string filename);
	void writeDimerActivating(std::vector<std::array<int, 4>> dimer_arr, std::string filename);
	void writeHomotetramers(std::vector<std::array<float, 2>>homotetramer_arr, std::string filename);
	void writeStates(std::vector<std::array<int, 36>> states_arr, std::string filename);

};

#endif //MARKOVNET_IO_H
