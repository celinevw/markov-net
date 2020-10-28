//
// Created by Celly on 23/10/2020.
//

#include "NetworkArray.h"

//TODO: check all probabilities of edges, combined with falling off

float RateToProbability(float x, float dt){
	return 1 - std::pow(1 - x, dt);
}

NetworkArray::NetworkArray(int l, int mm, int n1, int n2) {
	length = l;
	mismatchsite = mm;
	nickingsite1 = n1;
	nickingsite2 = n2;

	float dt = 0.01;			// seconds
	float S_on = 4.40e7;		// per M per second
	float L_on = 1.27e7;		// per M per second
	float H_on = 1e7;			// per M per second
	float S_change = 0.44; 		// per second
	float L_change = 1/8.0;		// per second
	float S_off = 1/32.2;		// per second
	float Sa_off = 1/683.0;		// per second
	float SL_off = 1/32.0;		// per second
	float SLa_off = 1/851.0;	// per second
	float SLH_off = 1/197.0;	// per second
	float S_conc = 1e-9;		// M
	float L_conc = 1e-9;		// M
	float H_conc = 1e-9;		// M

	const int numstates = 6;
	std::array<float,numstates-1> nextstate {S_on * S_conc * dt, RateToProbability(S_change, dt), L_on * L_conc * dt,
											 RateToProbability(L_change, dt), H_on * H_conc * dt};
	std::array<float,numstates-1> unbinding {S_off * dt, Sa_off * dt,SL_off * dt,
											 SLa_off * dt, SLH_off * dt};

	int i,j;
	for (int l = 0; l < numstates*numstates; l++) {
		i = l/numstates;
		j = l%numstates;

		// state1 goes to the next state
		if (l != 0 && j < numstates - 2 && l != 7 && l+1 != 7) {
			// Si-Si cannot be reached, so don't add edges to/from node 7
			transitions.at(l).at(l+1) = nextstate.at(j) * (1 - nextstate.at(i));
		}
		//state2 goes to the next state
		if (l != 0 && i < numstates - 2 && l != 7 && l + numstates != 7) {
			// Si-Si cannot be reached, so don't add edges to/from node 7
			transitions.at(l).at(l+numstates) = nextstate.at(j) * (1 - nextstate.at(i));
		}

		// A complex cannot fall of if it is not attached yet
		if(j != 0) {
			transitions.at(l).at(i) = unbinding.at(j-1);
		}
		if(i != 0) {
			transitions.at(l).at(j) = unbinding.at(i-1);
		}
	}
}

NetworkArray::NetworkArray() {
	NetworkArray(100, 50, 30, 90);
}