#include <fstream>
#include <cmath>
#include <array>
#include "ModelInstance.h"
#include "Distribution.h"


int main() {
	NetworkArray network(100, 30, 10, 80);		// Set up the network
	const int timesteps = 5000;
	const int num_sims = 10;

	UniformDistribution unif(0,1);
	auto *myarr_ptr = new std::array<float,num_sims*(2*timesteps+1)>;
	for (int i=0; i<num_sims*(2*timesteps+1); i++){
		myarr_ptr->at(i) = unif.getRandomNumber(); // CCC Neat use of the .at method, could probably be done even more neatly using a std::foreach but this is fine as is
	}

	auto *position_ptr = new std::array<std::array<int, timesteps+1>, num_sims>;
	auto *state_ptr = new std::array<std::array<int, timesteps+1>, num_sims>;

	auto it1 = myarr_ptr->begin(); // CCC WARNING this can get out of bounds if num_sims << myarr_ptr->size(). Just do for(auto itr = myarr_ptr->begin(); itr != myarr_ptr->end(); itr++) since you're never using i. Even a range-based for-loop will suffice here
	std::vector<ModelInstance> sims; // ToDo: put simulations on heap
	for(int i = 0; i < num_sims; i++){
		sims.emplace_back(network, *it1++);
	}

#pragma omp parallel
	{
#pragma omp for
		for (size_t i = 0; i < num_sims; i++){
			for (int j = 0; j < timesteps; j++){
				position_ptr->at(i)[j] = sims[i].getPosition();
				state_ptr->at(i)[j] = sims[i].getState();
				sims[i].transition(*it1++); // CCC Again, watch out for out-of-bounds issues. Don't use this technique often. Is it even necessary to have everything on the heap? If you just keep your ModelIteration object on the heap, stack size should not be an issue
				sims[i].setStep(*it1++);
			}

			position_ptr->at(i)[timesteps] = sims[i].getPosition();
			state_ptr->at(i)[timesteps] = sims[i].getState();

		}
	}

	std::ofstream out_pos;
	std::ofstream out_state;
	out_pos.open ("position.txt");
	out_state.open ("state.txt");
	for (int i = 0; i < timesteps+1; i++){
		for (int j=0; j < num_sims; j++) {
			out_pos << position_ptr->at(j)[i] << "\t"; // CCC Good to know the file is tab-delimited. You can also self-annotate the file by saving it as a .tsv (tab-separated value, related to the .csv file format)
			out_state << state_ptr->at(j)[i] << "\t";
		}
		out_pos << std::endl;
		out_state << std::endl;
	}

	return 0;
}
