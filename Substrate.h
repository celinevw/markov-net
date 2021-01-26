//
// Created by Celly on 23/11/2020.
//

#ifndef MARKOVNET_SUBSTRATE_H
#define MARKOVNET_SUBSTRATE_H

#include <vector>
#include <tuple>
#include <algorithm>
#include "ModelInstance.h"

class Substrate{
private:
	int id = 0;
	NetworkArray network;
	ParameterObj parameters;
	XoshiroCpp::Xoshiro128PlusPlus gen;
	std::uniform_real_distribution<> dist;
	std::uniform_int_distribution<> int_dist;
	std::vector<int> positions; // Vector containing all positions currently occupied
	int numcomplexes; // Number of complexes currently on the substrate
public:
	std::vector<ModelInstance> complexes;
	float currenttime{};
	bool mult_loading{};
	float nick1{};
	float nick2{};
	Substrate(NetworkArray &net, ParameterObj &par, bool allow_loading, std::uint64_t seed);
	Substrate();
	void assign(NetworkArray &net, ParameterObj &par, bool allow_loading, std::uint64_t seed);

	float getRandomXoshiro();
	int getRandomIntXoshiro();

	// Returns true if there are no complexes within footprint steps of pos, false otherwise
	bool positionFree(int pos, int footprint);

	// Returns vectors containing the nicking moments of all complexes
	std::array<float, 2> findNickingmoments();

	// Adds a complex to the substrate with probability of bindingchance
	void bindComplex(float bindingchance, int footprint);

	std::vector<std::vector<int>> main(int i);
};

#endif //MARKOVNET_SUBSTRATE_H
