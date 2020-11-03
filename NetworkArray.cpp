//
// Created by Celly on 23/10/2020.
//

#include "NetworkArray.h"

// TODO: check all probabilities of edges, combined with falling off

float RateToProbability(float x, float dt){
	return 1 - std::pow(1 - x, dt);
}

NetworkArray::NetworkArray(ParameterObj par) {
	length = 2770;
	if (par.subs == GT1A){
		mismatchsite = 1730;
		nickingsite1 = 690;
		nickingsite2 = -200;	//never reached
	}
	else if (par.subs == GT2A){
		mismatchsite = 1730;
		nickingsite1 = 690;
		nickingsite2 = 1760;
	}
	else if (par.subs == GT2B){
		mismatchsite = 750;
		nickingsite1 = 690;
		nickingsite2 = 1760;
	}

	float dt = 0.5;				// seconds, Check with modelinstance!
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
	float S_conc = par.S_conc;	// M
	float L_conc = par.L_conc;	// M
	float H_conc = par.H_conc;	// M

	std::array<float, 6> single_diff {0, 0, 43.0, 5.0, 5.0, 5.0};

	const int numstates = 6;
	std::array<float,numstates-1> nextstate {S_on * S_conc * dt, RateToProbability(S_change, dt), L_on * L_conc * dt,
											 RateToProbability(L_change, dt), H_on * H_conc * dt};
	std::array<float,numstates-1> unbinding {S_off * dt, Sa_off * dt,SL_off * dt,
											 SLa_off * dt, SLH_off * dt};

	// ToDo: check transitions
	transitions.fill({0});
	int i,j;
	for (int l = 0; l < numstates*numstates; l++) {
		i = l/numstates;
		j = l%numstates;

		// state1 goes to the next state
		if (l != 0 && j < numstates - 2 && l != 7 && l+1 != 7) {
			// Si-Si cannot be reached, so don't add edges to/from node 7
			transitions.at(l).at(l+1) = nextstate.at(j);
		}
		//state2 goes to the next state
		if (l != 0 && i < numstates - 2 && l != 7 && l + numstates != 7) {
			// Si-Si cannot be reached, so don't add edges to/from node 7
			transitions.at(l).at(l+numstates) = nextstate.at(i);
		}

		// A complex cannot fall off if it is not attached yet
		if(j != 0) {
			transitions.at(l).at(i*numstates) = unbinding.at(j-1);
		}
		if(i != 0) {
			transitions.at(l).at(j) = unbinding.at(i-1);
		}

		if(i == 1 || j == 1) {
			diffusion.at(l) = 0.0;
		}
		else if (single_diff.at(i) == 0){
			diffusion.at(l) = single_diff.at(j);
		}
		else if (single_diff.at(j) == 0){
			diffusion.at(l) = single_diff.at(i);
		}
		else{
			diffusion.at(l) = pow(pow(single_diff.at(i), -1) + pow(single_diff.at(j), -1), -1);
		}
	}
}

NetworkArray::NetworkArray() {
	NetworkArray(ParameterObj);
}