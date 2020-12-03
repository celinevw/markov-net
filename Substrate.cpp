//
// Created by Celly on 23/11/2020.
//

#include <fstream>
#include "Substrate.h"

void Substrate::assign(NetworkArray &net, ParameterObj &par, bool allow_loading, std::uint64_t seed) {
	network = net;
	parameters = par;
	mult_loading = allow_loading;
	currenttime = 0.0;
	nick1 = -1.0;
	nick2 = -1.0;

	gen = XoshiroCpp::Xoshiro128PlusPlus (seed);
	dist = std::uniform_real_distribution<> (0,1);
	int_dist = std::uniform_int_distribution<>(0, network.length);
	complexes.emplace_back(network, parameters, gen, 0, currenttime);
	positions.push_back(complexes.at(0).getPosition());
}

Substrate::Substrate(NetworkArray &net, ParameterObj &par, bool allow_loading, std::uint64_t seed){
	assign(net, par, allow_loading, seed);
}

Substrate::Substrate() {
	NetworkArray mynet;
	ParameterObj myparameters;
	std::uint64_t sd {1000};
	assign(mynet, myparameters, sd, false);
}

bool Substrate::positionFree(int pos) {
	return std::none_of(positions.begin(), positions.end(), [pos](int complex){return complex == pos;});
}

void Substrate::main() {
	std::cout << "Substrate" << std::endl;
	if (!mult_loading) {
		complexes.at(0).main(positions);
		nick1 = complexes.at(0).nick1;
		nick2 = complexes.at(0).nick2;
		return;
	}

	float dt_react = complexes.at(0).dt_react;
	float dt_diff = complexes.at(0).dt_diff;
	float bindingchance = network.transitions.at(1).at(7);
	int stepsperreaction = roundf(dt_react / dt_diff);
	numcomplexes = 1;

	for (int i = 1; i < (complexes.at(0).totaltime / dt_diff); i++) {
		currenttime = i * dt_diff;

		if(i % stepsperreaction == 0) {
			bindComplex(bindingchance);
			for (auto &protein : complexes) {
				if(protein.getState() != 0 && (protein.nick1<0 || protein.nick2<0)) {
					protein.reactionStep(i, positions);
				}
			}
		}

		for (auto &protein: complexes) {
			if(protein.getState() != 0) {
				protein.setStep(positions);
				protein.nicking();
				protein.currenttime = currenttime;
			}
		}

	}
	std::cout << numcomplexes << std::endl;
	currenttime = 0.0;

	findNickingmoments();
}


void Substrate::findNickingmoments() {
	std::array<std::vector<float>, 2> nicks;
	for (ModelInstance &protein : complexes) {
		if (protein.nick1 != -1) {
			nicks.at(0).push_back(protein.nick1);
		}
		if (protein.nick2 != -1) {
			nicks.at(1).push_back(protein.nick2);
		}
	}
	if (!nicks.at(0).empty()) { //if never nicked, leave at -1
		nick1 = *std::min_element(nicks.at(0).begin(), nicks.at(0).end());
	}
	if (!nicks.at(1).empty()) {
		nick2 = *std::min_element(nicks.at(1).begin(), nicks.at(1).end());
	}
}

void Substrate::bindComplex(float bindingchance) {
	float x = dist(gen);
	int binding_position = int_dist(gen);

	//binding moment: chance allows and mismatch not occupied
	if (x < bindingchance && !positionFree(binding_position)) {
		complexes.emplace_back(network, parameters, gen, numcomplexes, currenttime, binding_position);
		positions.push_back(complexes.at(numcomplexes).getPosition());
		numcomplexes += 1;
	}
}