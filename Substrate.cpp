//
// Created by Celly on 23/11/2020.
//

#include "Substrate.h"

void Substrate::assign(NetworkArray net, ParameterObj par, bool allow_loading) {
	network = net;
	parameters = par;
	mult_loading = allow_loading;
	currenttime = 0.0;
}

Substrate::Substrate(NetworkArray net, ParameterObj par, bool allow_loading){
	assign(net, par, allow_loading);
}

Substrate::Substrate() {
	NetworkArray mynet;
	ParameterObj myparameters;
	assign(mynet, myparameters, false);
}

void Substrate::main() {
	complexes.push_back(std::make_tuple(ModelInstance(network, parameters), 0.0));

	if(!mult_loading){
		std::vector<float> *numbers_ptr {}; // ToDo: get random numbers
		std::get<0>(complexes.at(0)).main(numbers_ptr);
		nick1 = std::get<0>(complexes.at(0)).nick1;
		nick2 = std::get<0>(complexes.at(0)).nick2;
		return;
	}


}