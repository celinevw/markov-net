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
	if (i < edges.size()) {
		return this->edges.at(i);
	}
}

void Network::addNode(Node n) {
	nodes.push_back(n);
	edges.push_back(std::vector<Edge>());
}

void Network::addEdge(Node n1, Node n2, double p) {
	auto it1 = std::find(nodes.begin(), nodes.end(), n1);
	int index = std::distance(nodes.begin(), it1);
	Edge to_add(n1, n2, p);
	if (std::find(edges.at(index).begin(), edges.at(index).end(), to_add) == edges.at(index).end()) {
		edges.at(index).push_back(to_add);
	}
}

void Network::printNet() {
	for (int i = 0; i < nodes.size(); i++) {
		std::cout << "\n Vertex" << i << ": ";
		for (auto e : edges.at(i)) {
			std::cout << e.getEndNode().getStateA().getStateName() << "&" << e.getEndNode().getStateB().getStateName()
					  << std::endl;
		}
	}
}

