//
// Created by Celly on 28/10/2020.
//

#ifndef MARKOVNET_IO_H
#define MARKOVNET_IO_H

#include <string>
#include <vector>
#include <fstream>
#include "ModelInstance.h"

class IO{
public:
	std::vector<float> read(int argc, char** arg);
	void write(ModelInstance model, std::string filename);
};

#endif //MARKOVNET_IO_H
