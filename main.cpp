#include <fstream>
#include <cmath>
#include <list>
#include "Network.h"
#include "NormalDistribution.h"
#include "ExponentialDistribution.h"

void writeEdges(){
	std::ofstream outfile;
	outfile.open("edges.txt");
	int numstates = 6;
	double nextstate[] = {0.1, 0.03, 0.25, 0.75, 0.04};

	for(int i=0; i<numstates; i++) {
		for (int j = 0; j < numstates - 1; j++) {
			/*
			// state1 goes to the next state
			if (i * 6 + j != 7 && i * 6 + (j + 1) != 7) { // Si-Si cannot be reached, so don't add edges to/from node 7
				outfile << i * 6 + j << " " << i * 6 + (j + 1) << " " << nextstate[j] * (1-nextstate[i]) << std::endl;
			}
			//state2 goes to the next state
			if (i + j * 6 != 7 && i + (j + 1) * 6 != 7) { // Si-Si cannot be reached, so don't add edges to/from node 7
				outfile << i + j * 6 << " " << i + (j + 1) * 6 << " " << nextstate[j] * (1 - nextstate[i]) << std::endl;
			}*/
			//both states go to the next state
			if (i * 6 + j != 7 && (i + 1) * 6 + (j + 1) != 7) {
				outfile << i * 6 + j << " " << (i + 1) * 6 + (j + 1) << " " << nextstate[j] * nextstate[i] << std::endl;
			}

		}
	}
	outfile.close();
}

Network makeNet(){
	// Create states
	std::vector<ComplexState> states;
	states.emplace_back("none", 0.0); //create ComplexState and immediatly add to end
	states.emplace_back("Si", 0.0);
	states.emplace_back("Sa", 0.01);
	states.emplace_back("SLi", 0.01);
	states.emplace_back("SLa", 0.01);
	states.emplace_back("SLH", 0.01);

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
	writeEdges(); //make sure all edges are in the file

	//Network MMR_net = makeNet();



	return 0;
}
