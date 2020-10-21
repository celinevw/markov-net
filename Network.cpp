//
// Created by Celly on 08/10/2020.
//

#include "Network.h"

Network::Network(std::vector<Node> nd) {
	this->nodes = nd;
	for (int i = 0; i < nodes.size(); i++) {
		edges.push_back(std::vector<Edge>());
	}
}

Network::Network() {
}

std::vector<Node> Network::getNodes() {
	return this->nodes;
}

std::vector<std::vector<Edge>> Network::getEdges() {
	return this->edges;
}

std::vector<Edge> Network::getEdgesOfNode(int i) {
	return this->edges.at(i);
}

void Network::addNode(Node n) {
	nodes.push_back(n);
	edges.push_back(std::vector<Edge>());
}

void Network::addEdge(int n1, int n2, double p) {
	Edge to_add(n1, n2, p);
	if (std::find(edges.at(n1).begin(), edges.at(n1).end(), to_add) == edges.at(n1).end()) {
		edges.at(n1).push_back(to_add);
	}
}

void Network::printNet() {
	for (int i = 0; i < nodes.size(); i++) {
		std::cout << "\n Vertex" << i << ": ";
		for (auto e : edges.at(i)) {
			std::cout << nodes.at(e.getEndNode()).getStateA().getStateName() << "&"
					  << nodes.at(e.getEndNode()).getStateB().getStateName() << std::endl;
		}
	}
}

