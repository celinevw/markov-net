#include <fstream>
#include <cmath>
#include <array>
#include "IO.h"
#include "Distribution.h"


int main(int argc, char ** arg) {
	IO myIO;
	ParameterObj myparameters;

	NetworkArray network(myparameters);		// Set up the network
	const int num_sims = 10;

	UniformDistribution unif(0,1);
	auto *myarr_ptr = new std::array<float,num_sims>;
	for (int i=0; i<num_sims; i++){
		myarr_ptr->at(i) = unif.getRandomNumber(); // CCC Neat use of the .at method, could probably be done even more neatly using a std::foreach but this is fine as is
	}

	ModelInstance myModel(network, myparameters);
/*	std::vector<ModelInstance*> sims; // create vector of pointers to model objects
	for (int i=0; i<num_sims; i++){
		sims.push_back(new ModelInstance(network, myparameters));
	}

#pragma omp parallel
	{
#pragma omp for
		for (size_t i = 0; i < num_sims; i++){
			sims.at(i)->main();
		}
	}

	std::string filepath = "modelOut.tsv";
	std::ofstream out;
	out.open(filepath);
	out << "output" << std::endl;
	for (ModelInstance* model: sims){
		myIO.write(*model, filepath);
	}*/

	return 0;
}
