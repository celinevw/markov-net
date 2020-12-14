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

bool Substrate::positionFree(int pos, int footprint) {
	return std::none_of(positions.begin(), positions.end(), [pos, footprint](int complex){
		return std::abs(complex - pos) <= footprint/2;
	});
}

std::array<float, 2> Substrate::findNickingmoments() {
	std::array<std::vector<float>, 2> nicks;
	for (ModelInstance &protein : complexes) {
		if (protein.nick1 != -1) {
			nicks.at(0).push_back(protein.nick1);
		}
		if (protein.nick2 != -1) {
			nicks.at(1).push_back(protein.nick2);
		}
	}
	std::array<float, 2> nickingmoments {-1, -1};
	if (!nicks.at(0).empty()) { //if never nicked, leave at -1
		nickingmoments.at(0) = *std::min_element(nicks.at(0).begin(), nicks.at(0).end());
	}
	if (!nicks.at(1).empty()) {
		nickingmoments.at(1) = *std::min_element(nicks.at(1).begin(), nicks.at(1).end());
	}
	return nickingmoments;
}

void Substrate::bindComplex(float bindingchance, int footprint) {
	if (!mult_loading){
		return;
	}
	float x = dist(gen);
	int binding_position = int_dist(gen);

	//binding moment: chance allows and mismatch not occupied
	if (x < bindingchance && positionFree(binding_position, footprint)) {
		complexes.emplace_back(network, parameters, gen, numcomplexes, currenttime, binding_position);
		positions.push_back(complexes.at(numcomplexes).getPosition());
		numcomplexes += 1;
		// std::cout << numcomplexes << "\t" << currenttime << std::endl;
	}
}

void Substrate::main() {
	if (mult_loading) {
		complexes.emplace_back(network, parameters, gen, 0, currenttime, int_dist(gen));
	}
	else {
		complexes.emplace_back(network, parameters, gen, 0, currenttime, network.mismatchsite);
	}

	positions.push_back(complexes.at(0).getPosition());

	float dt_react = complexes.at(0).dt_react;
	float dt_diff = complexes.at(0).dt_diff;
	float bindingchance = network.transitions.at(1).at(7);
	int stepsperreaction = roundf(dt_react / dt_diff);
	int footprint = complexes.at(0).mutS_footprint;
	numcomplexes = 1;
	std::array<float, 2> nickingmoments{-1, -1};

	/*
	std::ofstream pos_str;
	std::string pos_file = "positions.tsv";
	pos_str.open(pos_file);
	pos_str << parameters.S_conc << "\t"  << parameters.L_conc << "\t" << parameters.H_conc << "\t" << parameters.top << "\t" << parameters.subs << "\t" << mult_loading << std::endl;
	*/

	for (int i = 1; i < (complexes.at(0).totaltime / dt_diff); i++) {
		/*
		for (int &protein : positions) {
			pos_str << protein << "\t";
		}
		pos_str << std::endl;

		int at_mismatch = 0;
		for (int &protein : positions) {
			if (protein == 750) {
				at_mismatch += 1;
			}
		}
		if (at_mismatch > 1 ){
			std::cout << "multiple at mismatch" << std::endl;
		}
		*/

		currenttime = i * dt_diff;

		if(i % stepsperreaction == 0) {
			nickingmoments = findNickingmoments();
			if (std::none_of(nickingmoments.begin(), nickingmoments.end(), [](float i){ return i < 0; })) {
				break;
			}
			bindComplex(bindingchance, footprint);
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
	/*
	pos_str << numcomplexes << std::endl;
	pos_str.close();
	*/
	// std::cout << currenttime << std::endl;
	nick1 = nickingmoments.at(0);
	nick2 = nickingmoments.at(1);
}
