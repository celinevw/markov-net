#include <array>
#include "IO.h"
#include "Distribution.h"


int main(int argc, char ** arg) {
	IO myIO;
	ParameterObj myparameters;

	NetworkArray network(myparameters);		// Set up the network
	const int num_sims = 2;

	UniformDistribution unif(0,1);
	std::array<std::vector<float>*, num_sims> arr_per_sim{};
	//ToDo: get right amount of random numbers
	for (auto & it : arr_per_sim) {
		auto *myarr_ptr = new std::vector<float>;
		for (int i = 0; i < 650.0*(1/0.5 + 2/(100e-6)); i++) {
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

	std::string filepath = "modelOut.tsv";
	std::ofstream out;
	out.open(filepath);
	out.close();
	for (ModelInstance* model: sims){
		myIO.write(*model, filepath);
	}

	return 0;
}
