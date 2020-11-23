//
// Created by Celly on 23/10/2020.
//

#include "NetworkArray.h"

float RateToProbability(float x, float dt){
	return 1 - std::pow(1 - x, dt);
}

void NetworkArray::assign(ParameterObj par) {
	length = 2770;
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

	dt_react = 0.025;				// seconds, Check with modelinstance and number of random numbers in main!
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
	bool onlydimers = false;

	//D in micrometer^2/s
	std::array<float, 6> single_diff {0, 0.036, 0.043, 0.005, 0.005, 0.005};

	const int numstates = 6;
	std::array<float,numstates-1> nextstate {S_on * S_conc * dt_react, RateToProbability(S_change, dt_react), L_on * L_conc * dt_react,
											 RateToProbability(L_change, dt_react), H_on * H_conc * dt_react};
	std::array<float,numstates-1> unbinding {S_off * dt_react, Sa_off * dt_react,SL_off * dt_react,
											 SLa_off * dt_react, SLH_off * dt_react};

	transitions.fill({0});

	float i_next, i_unbind, j_next, j_unbind;
	int i,j;
	for (int l = 0; l < numstates*numstates; l++) {
		i = l/numstates;
		j = l%numstates;

		i_next = i < numstates - 1 ? (1 - nextstate.at(i)) : 1;	// if not at SLH, p of not going to the next state
		i_unbind = i > 0 && i < numstates ? (1 - unbinding.at(i - 1)) : 1;	// if not 'none', p of falling off
		j_next = j < numstates - 1 ? (1 - nextstate.at(j)) : 1;
		j_unbind = j > 0 && j < numstates? (1 - unbinding.at(j - 1)) : 1;

		// state1 goes to the next state
		if (l != 0 && j < numstates - 1) {
			transitions.at(l).at(l+1) = nextstate.at(j) * i_next * i_unbind * j_unbind;
		}

		if(!onlydimers){
			//state2 goes to the next state
			if (l != 0 && i < numstates - 1) {
				transitions.at(l).at(l+numstates) = nextstate.at(i) * j_next * i_unbind * j_unbind;
			}
		}

		/* Only one dimer can go to the next state. Both happening is not allowed, so don't add transition and ignore.
		 * Binding or conformational change only occurs if the complex does not fall off.
		 * A complex cannot fall off if it is not attached yet
		 */

		if(j != 0) {
			transitions.at(l).at(i*numstates) = unbinding.at(j-1);
		}
		if(i != 0) {
			transitions.at(l).at(j) = unbinding.at(i-1);
		}

		if (single_diff.at(i) == 0){
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

NetworkArray::NetworkArray(ParameterObj par) {
	assign(par);
}
NetworkArray::NetworkArray() {
	ParameterObj myparameters;
	assign(myparameters);
}