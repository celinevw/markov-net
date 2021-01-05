//
// Created by Celly on 20/10/2020.
//

#include "ModelInstance.h"

void ModelInstance::assign(NetworkArray &net, ParameterObj &par, XoshiroCpp::Xoshiro128PlusPlus &rng, int ind, float time, int pos) {
	network = net;
	position = pos >= 0 ? pos : network.mismatchsite; //if position not -1, otherwise mismatchsite
	state = 1;						// graph is symmetric, so let all start in state 1
	dt_react = net.dt_react; 						// for reaction, check dt with networkarray for probabilities
	dt_diff = 100e-6;				// for diffusion
	totaltime = 90;
	updateStep();
	nick1 = -1;
	nick2 = -1;
	currenttime = time;
	passed_mismatch = false;
	p_activate = 1;
	p_nick = 1;
	topology = par.top;
	my_index = ind;
	mutS_footprint = 13;

	dimersactive = std::array<std::vector<float>, 2> {};
	homotetramer = std::vector<float>{};
	states = std::vector<int>(totaltime/dt_react, 0);
	gen = rng;
	dist = std::uniform_real_distribution<> (0,1);
}

ModelInstance::ModelInstance(NetworkArray &net, ParameterObj &par, XoshiroCpp::Xoshiro128PlusPlus &rng, int ind, float start, int pos) {
	assign(net, par, rng, ind, start, pos);
}

ModelInstance::ModelInstance() {
	NetworkArray myNet;
	ParameterObj myPars;
	XoshiroCpp::Xoshiro128PlusPlus rng (1000);
	assign(myNet, myPars, rng, 0, 0.0, -1);
}

int ModelInstance::getState() const {
	return this->state;
}

int ModelInstance::getPosition() const {
	return position;
}

/* Randomly decide direction. Check whether new position lies within boundaries
 * If so, update step, else don't.
 */
void ModelInstance::setStep(std::vector<int> &positions) {
	if (stepsize == 0) {
		return;
	}
	float x = dist(gen);
	// Choose direction
	int direction;
	int edge = 0;
	int newposition = position;
	if(x < 0.5){
		direction = -1;
	}
	else{
		direction = 1;
	}

	// If not stepping off DNA, add step to position
	if(position + direction * stepsize >= 0 && position + direction * stepsize < network.length){
		newposition = position + (direction * stepsize);
	}
	else if (topology == circular){ // go to the "other side"
		edge = (position + direction * stepsize < 0) ? -1 : 1; //edge = -1 for low>high position, +1 for high>low
		newposition = (position + direction * stepsize + network.length) % network.length;
	}
	else if (topology == linear){ // fall off, go to state 0
		state = 0;
		newposition = -30;
		updateStep();
	} // if endblocked, do not take a step.

	// if moving, check whether the step can be made. Update position and passed_mismatch
	if(newposition != position && stepPossible(positions, newposition, edge)) {
		position = newposition;
		positions.at(my_index) = position;

		// if mismatch was not passed yet, update passed_mismatch
		if (!passed_mismatch) {
			passed_mismatch = (state / 6 == 1 || state % 6 == 1) &&
							  (std::abs((position - network.mismatchsite)) < stepsize);

			// if now near mismatch and step is possible, go to mismatchsite and set stepsize to 0. If mismatch cannot
			// be reached, set passed_mismatch back to false
			if(passed_mismatch & stepPossible(positions, network.mismatchsite, 0)) {
				// std::cout << my_index << " passed mismatch " << currenttime << std::endl;
				stepsize = 0;
				position = network.mismatchsite;
				positions.at(my_index) = position;
			}
			else {
				passed_mismatch = false;
			}
		}
	}
}

bool ModelInstance::stepPossible(std::vector<int> &positions, int newposition, int edge) const {
	// If falling off the substrate or this is the only complex, step is always possible
	if (newposition < 0 || positions.size()==1) {
		return true;
	}

	// Iterate over all complexes, check whether taking the step does not bring them too close to each other.
	// If passing an edge on circular susbtrate, check both low and high positions of the substrate.
	for (int i = 0; i < positions.size(); i++) {
		if (positions.at(i) < 0 ){
			continue;
		}
		if (i != my_index && std::abs(newposition - positions.at(i)) < mutS_footprint){
			return false;
		}
		if (edge == 0 && i!= my_index && ((newposition  <= positions.at(i) && positions.at(i) < position)
			|| (position < positions.at(i) && positions.at(i) <= newposition))) {
			return false;
		}
		else if (edge == -1 && i != my_index &&
		((positions.at(i)>=0 && positions.at(i) <= position) || positions.at(i) >= newposition)) {
			return false;
		}
		else if (edge == 1 && i != my_index &&
		(positions.at(i) > position || (positions.at(i) >=0 && positions.at(i) <= newposition))) {
			return false;
		}
	}
	return true;
}

/* Choose transition to follow based on random number x.
 */
void ModelInstance::transition(std::vector<int> &positions) {
	float x = dist(gen);
	// If there are no outgoing edges, then stay in this state
	if (std::accumulate(network.transitions.at(state).begin(), network.transitions.at(state).end(), 0.0) == 0){
		return;
	}

	// Calculate partial sums for all indices. If generated number is smaller than one, take the corresponding transition.
	std::array<float, 36> cumulative{};
	std::partial_sum(network.transitions.at(state).begin(), network.transitions.at(state).end(), cumulative.begin());
	int index = 0;
	bool Si_bound;
	for (float threshold : cumulative){
		if (x < threshold){
			Si_bound = index / 6 == 1 || index % 6 == 1;

			// dimer 1 inactive and bound to mismatch but transitioning, don't transition
			if (passed_mismatch && state % 6 == 1 && index == state + 1) {
				break;
			} // dimer 2 inactive and bound to mismatch but transition, don't transition
			else if (passed_mismatch && state / 6 == 1 && index == state + 6) {
				break;
			}

			state = index;
			// std::cout << "state:" << state << std::endl;

			// not passed mismatch, or stationary Si falls of then update stepsize.
			if (! passed_mismatch || (passed_mismatch && !Si_bound)) {
				updateStep();
			}

			// If new state is 0, go to position -30 so it will be ignored by other complexes.
			if (state == 0) {
				positions.at(my_index) = -30;
			}
			break;
		}
		index++;
	}
}

void ModelInstance::activateS(){
	float x = dist(gen);
	if (x >= p_activate){ // clamp failure
		return;
	}

	// if only complex 1 can activate, or choose randomly between the two
	if ((state / 6 != 1) || (state % 6 == 1 && x < (p_activate / 2))) {
		state = state + 1;
		// dimersactive.at(0).push_back(currenttime);
		updateStep();
		// std::cout << my_index << " activate complex 1 " << currenttime << std::endl;
	}
	else { // if only complex 2 can activate, or choose randomly
		state = state + 6;
		// dimersactive.at(1).push_back(currenttime);
		updateStep();
		// std::cout << my_index << " activate complex 2 " << currenttime << std::endl;
	}
}

void ModelInstance::nicking(){
	float x = dist(gen);
	// If one complex is SLH, possible nicking if not nicked yet
	if (state > 29 || state % 6 == 5){
		// Nicking site 1 if close enough and not nicked yet and allowed by p_nick
		if (std::abs((position - network.nickingsite1)) < stepsize && nick1 < 0 && x < p_nick){
			nick1 = currenttime;
			// std::cout << my_index << "\t" << currenttime << "\t" << 1 << std::endl;
		}
		// Nicking site 2 if close enough and not nicked yet and allowed by p_nick
		else if (std::abs((position - network.nickingsite2)) < stepsize && nick2 < 0 && x < p_nick){
			nick2 = currenttime;
			// std::cout << my_index << "\t" << currenttime << "\t" << 2 << std::endl;
		}
	}
}

void ModelInstance::updateStep() {
	// Calculate stepsize per dt_diff for current state
	stepsize = round(std::sqrt(2*network.diffusion.at(state)*dt_diff)*10000/3.4); //micrometer->armstrong->bp
}

void ModelInstance::reactionStep(int timeindex, std::vector<int> &positions) {
	int oldstate = state;
	int stepsperreaction = roundf(dt_react / dt_diff);

	// Save states over time
	// states.at(timeindex/stepsperreaction) = state;

	// Always check transitions, even if bound to mismatch
	transition(positions);

	// If bound at the mismatch, possibly activate inactive MutS dimer
	if (passed_mismatch && dist(gen) < network.activationS) {
		activateS();
		// std::cout << "activated S " << currenttime << std::endl;
		passed_mismatch = false;
	}

	/*
	// Save moments when becoming homo/hetero-type complex
	if (state / 6 == state % 6 ^ oldstate / 6 == oldstate % 6) {
		homotetramer.push_back(currenttime);
	}
	if (oldstate / 6 == oldstate % 6 && state == 0) {
		homotetramer.push_back(currenttime);
	}*/
}

/* main method, one run of a model instance
 */
void ModelInstance::main(std::vector<int> &positions) {
	// std::cout << currenttime << "\tModelinstance" << std::endl;
	int stepsperreaction = roundf(dt_react / dt_diff);
	int i = currenttime / dt_diff;

	while (dt_diff * i < totaltime && (nick1<0 || nick2<0) && state != 0) { //
		currenttime = dt_diff * i; //update only needed when time may be used
		setStep(positions);
		nicking();

		if(i % stepsperreaction == 0) {
			reactionStep(i, positions);
		}
		i++;
	}
}
