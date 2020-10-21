//
// Created by Celly on 07/10/2020.
//

#ifndef MARKOVNET_EDGE_H
#define MARKOVNET_EDGE_H

#include "Node.h"

class Edge{
private:
	int startNode;
	int endNode;
	double p; // probability of transition
public:
	Edge(int startNode, int endNode, double probability);
	Edge();
	int getStartNode();
	int getEndNode();
	double getP();

	friend bool operator==(Edge e1, Edge e2);
};

#endif //MARKOVNET_EDGE_H
