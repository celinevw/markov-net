//
// Created by Celly on 07/10/2020.
//

#include "Edge.h"

Edge::Edge(int s, int e, double probability){
	if (s != e) {
		startNode = s;
		endNode = e;
		p = probability;
	}
	else{
		Edge();
	}
}

Edge::Edge(){
	startNode = -1;
	endNode = -1;
}

int Edge::getStartNode() {
	return this->startNode;
}

int Edge::getEndNode() {
	return this->endNode;
}

double Edge::getP() {
	return this->p;
}

/* Return true if startnodes and endnodes are equal
 */
bool operator==(Edge e1, Edge e2){
	return (e1.getStartNode()==e2.getStartNode() && e1.getEndNode()==e2.getEndNode());
}