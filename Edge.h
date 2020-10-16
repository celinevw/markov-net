//
// Created by Celly on 07/10/2020.
//

#ifndef MARKOVNET_EDGE_H
#define MARKOVNET_EDGE_H

#include "Node.h"

class Edge{
private:
	Node startNode;
	Node endNode;
	double p; // probability of transition
public:
	Edge(Node startNode, Node endNode, double probability);
	Edge();
	Node getStartNode();
	Node getEndNode();
	double getP();

	friend bool operator==(Edge e1, Edge e2);
};

#endif //MARKOVNET_EDGE_H
