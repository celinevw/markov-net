//
// Created by Celly on 08/10/2020.
//

#ifndef MARKOVNET_NETWORK_H
#define MARKOVNET_NETWORK_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <iterator>
#include "Edge.h"

class Network{
private:
	std::vector <Node> nodes;
	std::vector <std::vector<Edge>> edges;

public:
	Network(std::vector<Node> nd);
	Network();
	std::vector <Node> getNodes();
	std::vector <std::vector<Edge>> getEdges();
	std::vector<Edge> getEdgesOfNode(int i);
	void addNode(Node n);
	void addEdge(int n1, int n2, double p);
	void printNet();

};

#endif //MARKOVNET_NETWORK_H
