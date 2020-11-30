#include <array>
#include "IO.h"
#include "Distribution.h"


int main(int argc, char ** arg) {
	IO myIO;
	ParameterObj myparameters = myIO.read(argc, arg);

	NetworkArray network(myparameters);		// Set up the network
	const int num_sims = 1000;
	bool multiple_loading = false;

	UniformDistribution unif(0,1);
	std::vector<Substrate*> sims; // create vector of pointers to model objects
	for (int i=0; i<num_sims; i++){
		sims.push_back(new Substrate(network, myparameters, multiple_loading, unif.getRandomNumber()*1000));
	}

	float totaltime = sims.at(0)->complexes.at(0).totaltime;
	float dt_reaction = sims.at(0)->complexes.at(0).dt_react;
	float dt_diffusion = sims.at(0)->complexes.at(0).dt_diff;
	float dt_plot = 0.1;


#pragma omp parallel
	{
#pragma omp for
		for (size_t i = 0; i < num_sims; i++){
			sims.at(i)->main();
		}
	}

	const int numtimesteps = 3000;
	std::vector<std::array<float, 4>> nicking_arr(numtimesteps, std::array<float, 4>{});
	bool nicked1;
	bool nicked2;

	// Create table nicking fractions over time
	for (Substrate* model: sims){
		for (int i = 0; i< numtimesteps; i++) {
			nicked1 = model->nick1 >0 && model->nick1 < i*dt_plot;
			nicked2 = model->nick2 >0 && model->nick2 < i*dt_plot;
			if (!nicked1 && !nicked2){
				nicking_arr.at(i).at(0) += 1.0 / num_sims;
			}
			if (nicked1 && !nicked2){
				nicking_arr.at(i).at(1) += 1.0 / num_sims;
			}
			if (!nicked1 && nicked2){
				nicking_arr.at(i).at(2) += 1.0 / num_sims;
			}
			if (nicked1 && nicked2){
				nicking_arr.at(i).at(3) += 1.0 / num_sims;
			}
		}
	}

	// Create table activations over time
	bool first_bound;
	bool second_bound;
	std::vector<std::array<int, 4>> boundarr(numtimesteps, std::array<int, 4>{});
	for (Substrate* model: sims){
		first_bound = false;
		second_bound = false;
		auto it1 = model->complexes.at(0).dimersactive.at(0).begin();
		auto it2 = model->complexes.at(0).dimersactive.at(1).begin();
		for (int i = 0; i< numtimesteps; i++) {
			if (it1 != model->complexes.at(0).dimersactive.at(0).end() && *it1 <= i * dt_plot){
				first_bound = !first_bound;
				it1++;
			}
			else if(first_bound && model->currenttime < i * dt_plot){
				first_bound = false;
			}
			if (it2 != model->complexes.at(0).dimersactive.at(1).end() && *it2 <= i * dt_plot){
				second_bound = !second_bound;
				it2++;
			}
			else if(second_bound && model->currenttime < i * dt_plot){
				second_bound = false;
			}
			if (!first_bound && !second_bound){
				boundarr.at(i).at(0) += 1;
			}
			if (first_bound && !second_bound){
				boundarr.at(i).at(1) += 1;
			}
			if (!first_bound && second_bound){
				boundarr.at(i).at(2) += 1;
			}
			if (first_bound && second_bound){
				boundarr.at(i).at(3) += 1;
			}
		}
	}

	// Create table homo/heterodimers over time
	bool ishomotetramer;
	std::vector<std::array<float,2>> homotetramer_arr(numtimesteps, std::array<float, 2>{});
	for(Substrate * model: sims){
		ishomotetramer = false;
		auto it1 = model->complexes.at(0).homotetramer.begin();
		for (int i = 0; i< numtimesteps; i++) {
			if (it1 != model->complexes.at(0).homotetramer.end() && *it1 <= i * dt_plot){
				ishomotetramer = !ishomotetramer;
				it1++;
			}
			if(ishomotetramer){
				homotetramer_arr.at(i).at(0) += 1;
			}
			else if(model->currenttime >= i * dt_plot){
				homotetramer_arr.at(i).at(1) += 1;
			}
		}
	}

	// Create table states over tine
	std::vector<std::array<int, 36>> states_arr(numtimesteps, std::array<int, 36>{});
	int index_statesvector;
	int currentstate;
	for (Substrate * model: sims) {
		currentstate = 1;
		for (int i=0; i<numtimesteps; i++){
			index_statesvector = float(i)*dt_plot/dt_reaction;
			if (currentstate > 0) {
				currentstate = model->complexes.at(0).states.at(index_statesvector);
			}
			states_arr.at(i).at(currentstate) += 1;
		}
	}

	std::ofstream out;

	// Save nicking moments
	std::string nickingmoment_file = "nicking_moments.tsv";
	out.open(nickingmoment_file);
	out << totaltime << "\t" << dt_plot << "\t" << num_sims << std::endl;
	out.close();
	myIO.momentsNicking(sims, nickingmoment_file);

	// Save nicking fractions over time
	std::string nicking_file = "nicking.tsv";
	out.open(nicking_file);
	out << totaltime << "\t" << dt_plot << "\t" << num_sims << std::endl;
	out << myparameters.S_conc << "\t" << myparameters.L_conc << "\t" << myparameters.H_conc << "\t" << myparameters.top << "\t" << myparameters.subs << std::endl;
	out.close();
	myIO.writeNicking(nicking_arr, nicking_file);

	/*
	std::string singleState_file = "statesSingle.tsv";
	out.open(singleState_file);
	out.close();
	myIO.singleStates(&sims.at(0)->complexes.at(0), singleState_file);

	std::string states_file = "states.tsv";
	out.open(states_file);
	out << totaltime << "\t" << dt_plot << "\t" << num_sims << std::endl;
	out.close();
	myIO.writeStates(states_arr, states_file);
	*/

	std::cout << "finished" << std::endl;
	return 0;
}
