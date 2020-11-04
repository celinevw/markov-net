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
	void write(ModelInstance model, std::string filename);
	void writeTransitions(NetworkArray net);
};

#endif //MARKOVNET_IO_H
