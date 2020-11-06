#include <array>
#include "IO.h"
#include "Distribution.h"


int main(int argc, char ** arg) {
	IO myIO;
	ParameterObj myparameters = myIO.read(argc, arg);

	NetworkArray network(myparameters);		// Set up the network
	const int num_sims = 5;
	float totaltime = 600;					// check with ModelInstance
	float dt_output = 0.1;

	UniformDistribution unif(0,1);
	std::array<std::vector<float>*, num_sims> arr_per_sim{};
	//ToDo: get right amount of random numbers
	for (auto & it : arr_per_sim) {
		auto *myarr_ptr = new std::vector<float>;
		for (int i = 0; i < (totaltime+30.0)*(1/0.5 + 2/(100e-6)); i++) { //hardcoded :(
			myarr_ptr->push_back(unif.getRandomNumber());
		}
		it = myarr_ptr;
	}

	std::vector<ModelInstance*> sims; // create vector of pointers to model objects
	for (int i=0; i<num_sims; i++){
		sims.push_back(new ModelInstance(network, myparameters));
	}

#pragma omp parallel
	{
#pragma omp for
		for (size_t i = 0; i < num_sims; i++){
			sims.at(i)->main(arr_per_sim.at(i));
		}
	}

	const int numtimesteps = 6000;
	std::array<std::array<float, 4>, numtimesteps> outputarr{};
	outputarr.fill({});
	bool nicked1;
	bool nicked2;

	for (ModelInstance* model: sims){
		for (int i = 0; i< numtimesteps; i++) {
			nicked1 = model->nick1 >0 && model->nick1 < i*dt_output;
			nicked2 = model->nick2 >0 && model->nick2 < i*dt_output;
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

	std::string filepath = "modelOut.tsv";
	std::ofstream out;
	out.open(filepath);
	out.close();
	for (ModelInstance* model: sims){
		myIO.write(*model, filepath);
	}

	std::string filepath2 = "timestepsOut.tsv";
	std::ofstream out_timesteps;
	out_timesteps.open(filepath2);
	out_timesteps << totaltime << "\t" << dt_output << std::endl;
	for (auto timestep: outputarr){
		for (auto nicked:  timestep){
			out_timesteps << nicked << "\t";
		}
		out_timesteps << std::endl;
	}

	return 0;
}
