#include <array>
#include "IO.h"
#include "Distribution.h"


int main(int argc, char ** arg) {
	IO myIO;
	ParameterObj myparameters = myIO.read(argc, arg);

	NetworkArray network(myparameters);		// Set up the network
	const int num_sims = 50;

	std::vector<ModelInstance*> sims; // create vector of pointers to model objects
	for (int i=0; i<num_sims; i++){
		sims.push_back(new ModelInstance(network, myparameters));
	}

	float totaltime = sims.at(0)->totaltime;
	float dt_reaction = sims.at(0)->dt_react;
	float dt_diffusion = sims.at(0)->dt_diff;

	UniformDistribution unif(0,1);
	std::array<std::vector<float>*, num_sims> arr_per_sim{};
	for (auto & it : arr_per_sim) {
		auto *myarr_ptr = new std::vector<float>;
		for (int i = 0; i < (totaltime+30.0)*(1/dt_reaction + 2/dt_diffusion); i++) { //hardcoded :(
			myarr_ptr->push_back(unif.getRandomNumber());
		}
		it = myarr_ptr;
	}

#pragma omp parallel
	{
#pragma omp for
		for (size_t i = 0; i < num_sims; i++){
			sims.at(i)->main(arr_per_sim.at(i));
		}
	}

	const int numtimesteps = 3000;
	std::array<std::array<float, 4>, numtimesteps> outputarr{};
	outputarr.fill({});
	bool nicked1;
	bool nicked2;

	// Create table nicking fractions over time
	for (ModelInstance* model: sims){
		for (int i = 0; i< numtimesteps; i++) {
			nicked1 = model->nick1 >0 && model->nick1 < i*dt_reaction;
			nicked2 = model->nick2 >0 && model->nick2 < i*dt_reaction;
			if (!nicked1 && !nicked2){
				outputarr.at(i).at(0) += 1.0/num_sims;
			}
			if (nicked1 && !nicked2){
				outputarr.at(i).at(1) += 1.0/num_sims;
			}
			if (!nicked1 && nicked2){
				outputarr.at(i).at(2) += 1.0/num_sims;
			}
			if (nicked1 && nicked2){
				outputarr.at(i).at(3) += 1.0/num_sims;
			}
		}
	}

	// Create table activations over time
	bool first_bound;
	bool second_bound;
	std::array<std::array<int, 4>, numtimesteps> boundarr{};
	for (ModelInstance* model: sims){
		first_bound = false;
		second_bound = false;
		auto it1 = model->firstbound.begin();
		auto it2 = model->secondbound.begin();
		for (int i = 0; i< numtimesteps; i++) {
			if (it1 != model->firstbound.end() && *it1 <= i * dt_reaction){
				first_bound = !first_bound;
				it1++;
			}
			if (it2 != model->secondbound.end() && *it2 <= i * dt_reaction){
				second_bound = !second_bound;
				it2++;
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

	bool ishomotetramer;
	std::array<std::array<int,2>, numtimesteps> homotetramer_arr{};
	// Save homo/heterodimers over time
	for(ModelInstance * model: sims){
		ishomotetramer = false;
		auto it1 = model->homotetramer.begin();
		for (int i = 0; i< numtimesteps; i++) {
			if (it1 != model->homotetramer.end() && *it1 <= i * dt_reaction){
				ishomotetramer = !ishomotetramer;
				it1++;
			}
			if(ishomotetramer){
				homotetramer_arr.at(i).at(0) += 1;
			}
			else if(model->currenttime >= i * dt_reaction){
				homotetramer_arr.at(i).at(1) += 1;
			}
		}
	}

	// Save homo/heterotetramers over time
	std::string homotetramer = "homotetramers.tsv";
	std::ofstream homotetramer_out;
	homotetramer_out.open(homotetramer);
	homotetramer_out << totaltime << "\t" << dt_reaction << "\t" << num_sims << std::endl;
	for (auto timestep: homotetramer_arr){
		homotetramer_out << timestep << std::endl;
	}
	homotetramer_out.close();

	// Save homotetramer moments
	std::string homotetramer_moment = "homotetramers2.tsv";
	std::ofstream homotetramermoment_out;
	homotetramermoment_out.open(homotetramer_moment);
	homotetramermoment_out << totaltime << "\t" << dt_reaction << "\t" << num_sims << std::endl;
	for (auto model : sims){
		for (auto time : model->homotetramer) {
			homotetramermoment_out << time << "\t";
		}
		homotetramermoment_out << std::endl;
	}
	homotetramermoment_out.close();

	// Save active dimers over time
	std::string dimerbinding = "dimerBinding.tsv";
	std::ofstream bindingstream;
	bindingstream.open(dimerbinding);
	bindingstream << totaltime << "\t" << dt_reaction << "\t" << num_sims << std::endl;
	for (auto timestep: boundarr){
		for (auto bound:  timestep){
			bindingstream << bound << "\t";
		}
		bindingstream << std::endl;
	}
	bindingstream.close();

	// Save binding/unbinding moments
	std::string bindingmoments = "dimerBinding2.tsv";
	std::ofstream out_bindingmoments;
	out_bindingmoments.open(bindingmoments);
	for (ModelInstance* model: sims){
		for (auto i: model->firstbound){
			out_bindingmoments << i << "\t";
		}
		out_bindingmoments << std::endl;
		for (auto i: model->secondbound){
			out_bindingmoments << i << "\t";
		}
		out_bindingmoments << std::endl;
	}
	out_bindingmoments.close();

	// Save nicking moments
	std::string filepath = "modelOut.tsv";
	std::ofstream out;
	out.open(filepath);
	out.close();
	for (ModelInstance* model: sims){
		myIO.write(*model, filepath);
	}

	// Save nicking fractions over time
	std::string filepath2 = "timestepsOut.tsv";
	std::ofstream out_timesteps;
	out_timesteps.open(filepath2);
	out_timesteps << totaltime << "\t" << dt_reaction << "\t" << num_sims << std::endl;
	for (auto timestep: outputarr){
		for (auto nicked:  timestep){
			out_timesteps << nicked << "\t";
		}
		out_timesteps << std::endl;
	}

	std::cout << "finished" << std::endl;
	return 0;
}
