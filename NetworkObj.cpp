//
// Created by Celly on 08/10/2020.
//

#include "NetworkObj.h"

NetworkObj::NetworkObj(std::vector<Node> nd) {
	this->nodes = nd;
	for (int i = 0; i < nodes.size(); i++) {
		edges.push_back(std::vector<Edge>());
	}
}

NetworkObj::NetworkObj() {
}

std::vector<Node> NetworkObj::getNodes() {
	return this->nodes;
}

std::vector<std::vector<Edge>> NetworkObj::getEdges() {
	return this->edges;
}

std::vector<Edge> NetworkObj::getEdgesOfNode(int i) {
	return this->edges.at(i);
}

void NetworkObj::addNode(Node n) {
	this->nodes.push_back(n);
	this->edges.push_back(std::vector<Edge>());
}

/* Add edge to the correct vector.
 */
void NetworkObj::addEdge(int n1, int n2, double p) {
	Edge to_add(n1, n2, p);
	if (std::find(edges.at(n1).begin(), edges.at(n1).end(), to_add) == edges.at(n1).end()) {
		edges.at(n1).push_back(to_add);
	}
}

/* For each node, print edges.
 */
void NetworkObj::printNet() {
	for (int i = 0; i < nodes.size(); i++) {
		std::cout << "\n Vertex" << i << ": ";
		for (auto e : edges.at(i)) {
			std::cout << nodes.at(e.getEndNode()).getStateA().getStateName() << "&"
					  << nodes.at(e.getEndNode()).getStateB().getStateName() << std::endl;
		}
	}
}

