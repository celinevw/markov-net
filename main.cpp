#include <fstream>
#include <cmath>
#include <array>
#include "ModelInstance.h"

double RateToProbability(double x, double dt){
	return 1 - std::pow(1 - x, dt); // CCC Sorry for handing you the old version of the document, unbinding rates are also just f*dt
}

void writeEdges(){
	//TODO: check all probabilities of edges, combined with falling off
	//ToDo: make 1 method without writing to file
	double dt = 0.1;			// seconds
	double S_on = 4.40e7;		// per M per second
	double L_on = 1.27e7;		// per M per second
	double H_on = 1e7;			// per M per second
	double S_change = 0.44; 	// per second
	double L_change = 1/8.0;	// per second
	double S_off = 1/32.2;		// per second
	double Sa_off = 1/683.0;	// per second
	double SL_off = 1/32.0;		// per second
	double SLa_off = 1/851.0;	// per second
	double SLH_off = 1/197.0;	// per second
	double conc = 1e-9;			// M


	std::ofstream outfile;
	outfile.open("edges.txt"); // CCC Pass the path as a variable, or pass the outstream by reference
	int numstates = 6;
	double nextstate[] = {S_on * conc * dt, S_change * dt, L_on * conc * dt,
					   L_change * dt, H_on * conc * dt}; // CCC Why bare array and not std::array
	double unbinding[] = {RateToProbability(S_off, dt), RateToProbability(Sa_off, dt), RateToProbability(SL_off, dt),
						  RateToProbability(SLa_off, dt), RateToProbability(SLH_off, dt)}; // CCC Why bare array and not std::array

	for (int i = 0; i < numstates; i++) {

		for (int j = 0; j < numstates - 1; j++) {

			// state1 goes to the next state
			if ((i != 0 || j != 0) && i * numstates + j != 7 && i * numstates + (j + 1) != 7) { // Si-Si cannot be reached, so don't add edges to/from node 7
				outfile << i * numstates + j << " " << i * numstates + (j + 1) << " " << nextstate[j] << std::endl;
			}
			//state2 goes to the next state
			if ((i != 0 || j != 0) && i + j * numstates != 7 && i + (j + 1) * numstates != 7) { // Si-Si cannot be reached, so don't add edges to/from node 7
				outfile << i + j * numstates << " " << i + (j + 1) * numstates << " " << nextstate[j] << std::endl;
			}

		}
	}

	// Falling off
	for (int i = 0; i < numstates; i++) {
		for (int j = 0; j<numstates; j++){
			if (i * numstates + j != 0 && i * numstates + j != 7){
				outfile << i * numstates + j << " " << i * numstates << " " << unbinding[j-1];
				outfile << i + j * numstates << " " << i << " " << unbinding[j-1];
			}
		}
	}
	outfile.close();
}

void writeSmallNet(){
	double dt = 0.01;			// seconds
	double S_on = 4.40e7;		// per M per second
	double L_on = 1.27e7;		// per M per second
	double H_on = 1e7;			// per M per second
	double S_change = 0.44; 	// per second
	double L_change = 1/8.0;	// per second
	double S_off = 1/32.2;		// per second
	double Sa_off = 1/683.0;	// per second
	double SL_off = 1/32.0;		// per second
	double SLa_off = 1/851.0;	// per second
	double SLH_off = 1/197.0;	// per second
	double conc = 1e-9;			// CCC Concentration of what?? MutS I assume


	std::ofstream outfile;
	outfile.open("edges.txt");
	int numstates = 6;
	double nextstate[] = {S_on * conc * dt, RateToProbability(S_change, dt), L_on * conc * dt,
						  RateToProbability(L_change, dt), H_on * conc * dt};
	double unbinding[] = {RateToProbability(S_off, dt), RateToProbability(Sa_off, dt), RateToProbability(SL_off, dt),
						  RateToProbability(SLa_off, dt), RateToProbability(SLH_off, dt)}; // CCC Don't use bare arrays, those are usually not std::algorithm friendly

        // CCC You could use a single loop if you say i = I/3 and j = I%3
	for (int i = 0; i < 3; i++) {

		for (int j = 0; j < 3 - 1; j++) {

			// state1 goes to the next state
			if ((i != 0 || j != 0) && i * numstates + j != 7 && i * numstates + (j + 1) != 7) { // Si-Si cannot be reached, so don't add edges to/from node 7
				outfile << i * numstates + j << " " << i * numstates + (j + 1) << " " <<
				nextstate[j] * (1 - nextstate[i]) << std::endl;
			}
			//state2 goes to the next state
			if ((i != 0 || j != 0) && i + j * numstates != 7 && i + (j + 1) * numstates != 7) { // Si-Si cannot be reached, so don't add edges to/from node 7
				outfile << i + j * numstates << " " << i + (j + 1) * numstates << " " <<
				nextstate[j] * (1 - nextstate[i]) << std::endl;
			}

		}
	}

	// Falling off
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++){
			if (i * numstates + j != 0 && i * numstates + j != 7){
				// A complex cannot fall of if it is not attached yet
				if(j != 0) {
					outfile << i * numstates + j << " " << i * numstates << " " << unbinding[j - 1] << std::endl;
				}
				if(i != 0) {
					outfile << i * numstates + j << " " << j << " " << unbinding[i - 1] << std::endl;
				}
			}
		}
	}
	outfile.close();
}

Network makeNet(){
	// Create states
	std::vector<ComplexState> states;
	states.emplace_back("none", 0.0); 	// create ComplexState instance and immediatly add to end
	states.emplace_back("Si", 0.0);	// diffusion constant in micrometer^2/s
	states.emplace_back("Sa", 43000);
	states.emplace_back("SLi", 5000);
	states.emplace_back("SLa", 5000);
	states.emplace_back("SLH", 5000);

	//Create nodes
	std::vector <Node> nodes;
	for(const ComplexState& state1 : states){
		for(const ComplexState& state2 : states){
			nodes.emplace_back(state2, state1); // CCC I did not know this function, neat, I'll use it myself more often :)
		}
	}
	Network MMR_net(nodes);

	// Edges for one complex to the next state, read from file
	std::ifstream infile;
	infile.open("edges.txt");
	int n1, n2;
	double p;

	while(!infile.eof()){
		infile >> n1;
		infile >> n2;
		infile >> p;
		MMR_net.addEdge(n1, n2, p);
	}
	infile.close();

	return MMR_net;
}

int main() {
	writeSmallNet(); 					//make sure all edges are in the file
	Network MMR_net = makeNet();	// Set up the network
	const int timesteps = 5000;
	const int num_sims = 10;
	const int length_DNA = 100;

	UniformDistribution unif(0,1);
	auto *myarr_ptr = new std::array<float,num_sims*(2*timesteps+1)>;
	for (int i=0; i<num_sims*(2*timesteps+1); i++){
		myarr_ptr->at(i) = unif.getRandomNumber(); // CCC Neat use of the .at method, could probably be done even more neatly using a std::foreach but this is fine as is
	}

	auto *position_ptr = new std::array<std::array<int, timesteps+1>, num_sims>;
	auto *state_ptr = new std::array<std::array<int, timesteps+1>, num_sims>;

	auto it1 = myarr_ptr->begin(); // CCC WARNING this can get out of bounds if num_sims << myarr_ptr->size(). Just do for(auto itr = myarr_ptr->begin(); itr != myarr_ptr->end(); itr++) since you're never using i. Even a range-based for-loop will suffice here
	std::vector<ModelInstance> sims;
	for(int i = 0; i < num_sims; i++){
		sims.emplace_back(MMR_net, length_DNA, *it1++);
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
			out_pos << position_ptr->at(j)[i] << "\t"; /// CCC Good to know the file is tab-delimited. You can also self-annotate the file by saving it as a .tsv (tab-separated value, related to the .csv file format)
			out_state << state_ptr->at(j)[i] << "\t";
		}
		out_pos << std::endl;
		out_state << std::endl;
	}

	return 0;
}
