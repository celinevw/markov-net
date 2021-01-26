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
	// Returns true if no complexes are within footprint bp of position pos on the substrate
	return std::none_of(positions.begin(), positions.end(), [pos, footprint](int complex){
		return std::abs(complex - pos) <= footprint;
	});
}

std::array<float, 2> Substrate::findNickingmoments() {
	std::array<std::vector<float>, 2> nicks;
	// Iterate over all proteins and if it made a nick, push the nicking moment to the corresponding vector
	for (ModelInstance &protein : complexes) {
		if (protein.nick1 != -1) {
			nicks.at(0).push_back(protein.nick1);
		}
		if (protein.nick2 != -1) {
			nicks.at(1).push_back(protein.nick2);
		}
	}
	std::array<float, 2> nickingmoments {-1, -1};
	// If a nicking site is not nicked, leave the value at -1. Otherwise change to the smallest value in the vector
	if (!nicks.at(0).empty()) {
		nickingmoments.at(0) = *std::min_element(nicks.at(0).begin(), nicks.at(0).end());
	}
	if (!nicks.at(1).empty()) {
		nickingmoments.at(1) = *std::min_element(nicks.at(1).begin(), nicks.at(1).end());
	}
	return nickingmoments;
}

void Substrate::bindComplex(float bindingchance, int footprint) {
	// If it is a trapping experiment, never bind extra complexes
	if (!mult_loading){
		return;
	}
	float x = dist(gen);
	int binding_position = int_dist(gen);

	// If x is smaller than the chance of binding and the position is free, bind a complex
	if (x < bindingchance && positionFree(binding_position, footprint)) {
		complexes.emplace_back(network, parameters, gen, numcomplexes, currenttime, binding_position);
		positions.push_back(complexes.at(numcomplexes).getPosition());
		numcomplexes += 1;
		// std::cout << numcomplexes << "\t" << currenttime << std::endl;
	}
}

std::vector<std::vector<int>> Substrate::main(int i) {
	id = i;

	// If no multiple loading, all complexes start at the mismatch. Otherwise randomly choose a position.
	// If MutS concentration is 0, don't add any complexes
	if (parameters.S_conc <= 0){
		complexes.emplace_back(network, parameters, gen, 0, currenttime, network.mismatchsite);
		return std::vector<std::vector<int>> {};
	}
	if (mult_loading || network.mismatchsite < 0) {
		complexes.emplace_back(network, parameters, gen, 0, currenttime, int_dist(gen));
	}
	else {
		complexes.emplace_back(network, parameters, gen, 0, currenttime, network.mismatchsite);
	}

	positions.push_back(complexes.at(0).getPosition());

	float dt_react = complexes.at(0).dt_react;
	float dt_diff = complexes.at(0).dt_diff;
	float bindingchance = network.transitions.at(6).at(7);
	int stepsperreaction = roundf(dt_react / dt_diff);
	int footprint = complexes.at(0).mutS_footprint;

	int numGATC = 0;
	numGATC = parameters.subs == GT2A || parameters.subs == GT2B ? 2 : 1;

	numcomplexes = 1;
	std::array<float, 2> nickingmoments{-1, -1};
	std::vector<std::vector<int>> positions_vec;

	/*
	std::ofstream pos_str;
	std::string pos_file = "positions.tsv";
	pos_str.open(pos_file);
	pos_str << parameters.S_conc << "\t"  << parameters.L_conc << "\t" << parameters.H_conc << "\t" << parameters.top << "\t" << parameters.subs << "\t" << mult_loading << std::endl;
	*/

	// Keep iterating until total time is reached
	for (int i = 1; i < (complexes.at(0).totaltime / dt_diff); i++) {
		/*
		for (auto &protein : complexes) {
			pos_str << protein.getPosition() << "\t";
		}
		pos_str << std::endl;
		*/

		currenttime = i * dt_diff;

		// Reaction step occurs once in stepsperreaction diffusion steps
		if(i % stepsperreaction == 0) {
			nickingmoments = findNickingmoments();
			if (std::count_if(nickingmoments.begin(), nickingmoments.end(), [](float i){ return i >= 0; }) == numGATC) {
				break;
			}
			bindComplex(bindingchance, footprint);
			for (auto &protein : complexes) {
				if(protein.getState() != 0 && (protein.nick1<0 || protein.nick2<0)) {
					protein.reactionStep(i, positions);
				}
			}

			// Save all positions
			// positions_vec.push_back(positions);
		}

		// Iterate over proteins for diffusion and nicking
		for (auto &protein: complexes) {
			if(protein.getState() != 0) {
				protein.setStep(positions);
				protein.nicking(id);
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

	return positions_vec;
}
