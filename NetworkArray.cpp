//
// Created by Celly on 23/10/2020.
//

#include "NetworkArray.h"

float RateToProbability(float x, float dt){
	return 1 - std::pow(1 - x, dt);
}

void NetworkArray::assign(ParameterObj par) {
	length = 2770;
	// Save mismatch and nicking site positions depending on substrate type
	if (par.subs == GT1A){
		mismatchsite = 1730;
		nickingsite1 = 690;		// far site
		nickingsite2 = -200;	// never reached
	}
	else if (par.subs == GT2A){
		mismatchsite = 1730;
		nickingsite1 = 690;		// close site
		nickingsite2 = 1760;
	}
	else if (par.subs == GT2B){
		mismatchsite = 750;
		nickingsite1 = 690;
		nickingsite2 = 1760;
	}
	else if (par.subs == AT1A){
		mismatchsite = -200;	// never reached
		nickingsite1 = 690;		// far site
		nickingsite2 = -200;	// never reached
	}

	dt_react = 0.025;				// seconds, Check with modelinstance and number of random numbers in main!
	const float S_on = 4.40e7;		// per M per second
	const float L_on = 1.27e7;		// per M per second
	const float H_on = 1e7;			// per M per second
	const float S_change = 0.44; 		// per second
	const float L_change = 1 / 8.0;		// per second
	const float S_off = 1 / 32.2;		// per second
	const float Sa_off = 1 / 683.0;		// per second
	const float L_off = 1 / 32.0;		// per second
	const float La_off = 1 / 851.0;	// per second
	const float H_off = 1 / 197.0;	// per second
	const float S_conc = par.S_conc;	// M
	const float L_conc = par.L_conc;	// M
	const float H_conc = par.H_conc;	// M
	const bool tetramers = par.tetramer;

	activationS = S_change * dt_react;

	//D in micrometer^2/s
	std::array<float, 6> single_diff {0, 0.036, 0.043, 0.005, 0.005, 0.005};

	const int numstates = 6;
	std::array<float, numstates - 1> nextstate{S_on * S_conc * dt_react, 0, L_on * L_conc * dt_react / 2,
											   L_change * dt_react, H_on * H_conc * dt_react};
	if (!tetramers) {
		nextstate.at(2) = L_on * L_conc * dt_react;
	}

	std::array<float, numstates> transition_Soff {0.0, S_off * dt_react,
												  Sa_off * dt_react, Sa_off * dt_react,
												  Sa_off * dt_react, Sa_off * dt_react};
	std::array<float, numstates> transition_Loff {0.0, 0.0,
												  0.0, L_off * dt_react,
												  La_off * dt_react, La_off * dt_react};
	std::array<float, numstates> transition_Hoff {0.0, 0.0,
												  0.0, 0.0,
												  0.0, H_off * dt_react};
	transitions.fill({0});
	diffusion.fill({0});

	float i_next, j_next;
	int i,j;
	// Iterate over all states
	for (int l = 0; l < numstates*numstates; l++) {
		i = l / numstates;
		j = l % numstates;

		// Fill diffusion constants
		if (single_diff.at(i) == 0){
			diffusion.at(l) = single_diff.at(j);
		}
		else if (single_diff.at(j) == 0){
			diffusion.at(l) = single_diff.at(i);
		}
		else{
			diffusion.at(l) = pow(pow(single_diff.at(i), -1) + pow(single_diff.at(j), -1), -1);
		}

		// Add transitions for unbinding of a protein. Zeroes can be overwritten by next-state transitions
		transitions.at(l).at(j) = transition_Soff.at(i);
		transitions.at(l).at(i * numstates) = transition_Soff.at(j);
		transitions.at(l).at(2 * numstates + j) = transition_Loff.at(i);
		transitions.at(l).at(i * numstates + 2) = transition_Loff.at(j);
		transitions.at(l).at(4 * numstates + j) = transition_Hoff.at(i);
		transitions.at(l).at(i * numstates + 4) = transition_Hoff.at(j);

		// Add transitions for going to the next state. Only one dimer can go to the next state.
		// Both happening is not allowed, so don't add transition and ignore.
		i_next = i < numstates - 1 ? (1 - nextstate.at(i)) : 1;    // if not at SLH, p of not going to the next state
		j_next = j < numstates - 1 ? (1 - nextstate.at(j)) : 1;

		// state1 goes to the next state
		if (l != 0 && j < numstates - 1) {
			transitions.at(l).at(l + 1) = nextstate.at(j) * i_next;
		}

		//state2 goes to the next state, if tetramer. only one can transition.
		if (!onlydimers) {
			if (l != 0 && i < numstates - 1) {
				transitions.at(l).at(l + numstates) = nextstate.at(i) * j_next;
			}
		}

	}
}

NetworkArray::NetworkArray(ParameterObj par) {
	assign(par);
}
NetworkArray::NetworkArray() {
	ParameterObj myparameters;
	assign(myparameters);
}