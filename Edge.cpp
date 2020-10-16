//
// Created by Celly on 07/10/2020.
//

#include "Edge.h"

Edge::Edge(Node s, Node e, double probability){
	startNode = s;
	endNode = e;
	p = probability;
}

Edge::Edge(){
	startNode = Node();
	endNode = Node();
}

Node Edge::getStartNode() {
	return this->startNode;
}

Node Edge::getEndNode() {
	return this->endNode;
}

double Edge::getP() {
	return this->p;
}

bool operator==(Edge e1, Edge e2){
	return (e1.getStartNode()==e2.getStartNode() && e1.getEndNode()==e2.getEndNode() && e1.getP()==e2.getP());
}