//
// Created by Celly on 08/10/2020.
//

#include "Network.h"

Network::Network(std::vector<Node> nd){
	this->nodes = nd;
	for(int i=0; i<nodes.size(); i++){
		edges.push_back(std::vector<Edge>());
	}
}

Network::Network(){
}

std::vector <Node> Network::getNodes(){
	return this -> nodes;
}

void Network::addNode(Node n){
	nodes.push_back(n);
	edges.push_back(std::vector<Edge>());

}

void Network::addEdge(Node n1, Node n2, double p){
	auto it1 = std::find(nodes.begin(), nodes.end(), n1);
	int index = std::distance(nodes.begin(), it1);
	edges.at(index).push_back(Edge(n1, n2, p));

}

void Network::printNet() {
	for(int i = 0; i < nodes.size(); i++){
		std::cout << "\n Vertex" << i << ": ";
		for(auto e : edges.at(i)){
			std::cout << e.getEndNode().getStateA().getStateName() << "&" << e.getEndNode().getStateB().getStateName() << std::endl;
		}
	}
}

