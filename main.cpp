#include <fstream>
#include <cmath>
#include <array>
#include "IO.h"
#include "Distribution.h"


int main(int argc, char ** arg) {
	IO myIO;
	ParameterObj pars = myIO.read(argc, arg);

	NetworkArray network(pars);		// Set up the network
	const int timesteps = 5000;
	const int num_sims = 10;

	UniformDistribution unif(0,1);
	auto *myarr_ptr = new std::array<float,num_sims*(2*timesteps+1)>;
	for (int i=0; i<num_sims; i++){
		myarr_ptr->at(i) = unif.getRandomNumber(); // CCC Neat use of the .at method, could probably be done even more neatly using a std::foreach but this is fine as is
	}

	std::vector<ModelInstance*> sims; // create vector of pointers to model objects
	for (float & it : *myarr_ptr){
		sims.push_back(new ModelInstance(network, pars, it));
	}

#pragma omp parallel
	{
#pragma omp for
		for (size_t i = 0; i < num_sims; i++){
			sims.at(i)->main();
		}
	}

	std::string filepath = "modelOut.tsv";
	for (ModelInstance* model: sims){
		myIO.write(*model, filepath);
	}

	return 0;
}
