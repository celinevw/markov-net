#include <fstream>
#include <cmath>
#include <list>
#include "Network.h"
#include "NormalDistribution.h"
#include "ExponentialDistribution.h"

void writeEdges(){
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
	outfile.open("edges.txt");
	int numstates = 6;
	double nextstate[] = {S_on * conc * dt, S_change * dt, L_on * conc * dt,
					   L_change * dt, H_on * conc * dt};

	for (int i = 0; i < numstates; i++) {

		for (int j = 0; j < numstates - 1; j++) {

			// state1 goes to the next state
			if (i * numstates + j != 7 && i * numstates + (j + 1) != 7) { // Si-Si cannot be reached, so don't add edges to/from node 7
				outfile << i * numstates + j << " " << i * numstates + (j + 1) << " " << nextstate[j] << std::endl;
			}
			//state2 goes to the next state
			if (i + j * numstates != 7 && i + (j + 1) * numstates != 7) { // Si-Si cannot be reached, so don't add edges to/from node 7
				outfile << i + j * numstates << " " << i + (j + 1) * numstates << " " << nextstate[j] << std::endl;
			}

		}
	}
	for (int i = 0; i < numstates-1; i++) {
		outfile << nextstate[i] << " " ;
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
			nodes.emplace_back(state2, state1);
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
		MMR_net.addEdge(nodes.at(n1), nodes.at(n2), p);
	}
	infile.close();

	return MMR_net;
}

int main() {
	writeEdges(); 				//make sure all edges are in the file
	//Network MMR_net = makeNet();	// Set up the network





	return 0;
}
