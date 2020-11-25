//
// Created by Celly on 23/11/2020.
//

#include "Substrate.h"

void Substrate::assign(NetworkArray net, ParameterObj par, bool allow_loading) {
	network = net;
	parameters = par;
	mult_loading = allow_loading;
	currenttime = 0.0;
	complexes.emplace_back(network, parameters);
	nick1 = -1.0;
	nick2 = -1.0;
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
	if(!mult_loading){
		std::vector<float> *numbers_ptr {}; // ToDo: get random numbers
		complexes.at(0).main(numbers_ptr);
		nick1 = complexes.at(0).nick1;
		nick2 = complexes.at(0).nick2;
		return;
	}

	// ToDo: decide when to allow complexes binding
	//binding moment
	if (true) {
		// Adding a complex
		complexes.emplace_back(network, parameters, currenttime);
	}

	std::vector<float> *numbers_ptr {}; // ToDo: get random numbers
	std::array<std::vector<float>, 2> nicks;
	std::array<float,2> nicks_complex {-1.0, -1.0};
	for(ModelInstance protein : complexes){
		nicks_complex = protein.main(numbers_ptr);
		if (nicks_complex.at(0) != -1) {
			nicks.at(0).push_back(nicks_complex.at(0));
		}
		if (nicks_complex.at(0) != -1) {
			nicks.at(1).push_back(nicks_complex.at(1));
		}
	}
	if (!nicks.at(0).empty()) { //if never nicked, leave at -1
		nick1 = *std::min_element(nicks.at(0).begin(), nicks.at(0).end());
	}
	if (!nicks.at(1).empty()) {
		nick2 = *std::min_element(nicks.at(1).begin(), nicks.at(1).end());
	}

}