#include "stdafx.h"
#include <iostream>
#include "Node.h"

using namespace std;

string Node::getCountry() {
	return this->country;
}

string Node::getTransportType(string city) {
	return transportType;
}

double Node::edgeWeight(string city, int criteria) {
	double min = -1;
//	pair<string, int> myPair(city, criteria);
	//if we already have the info stored, dont calculate it again, it doesnt change
	//if (edgeWeightMap.find(myPair) != edgeWeightMap.end()) {
		//min = edgeWeightMap[myPair];
	//}else{
		double weight = 0;
		Edges edge;
		for (unsigned int i = 0; i < edgeMap[city].size(); i++) {
			edge = edgeMap[city].at(i);
			weight = edge.getWeight(criteria);
			if (weight < min || min == -1) {
				min = weight;
				transportType = edge.transport;
			}
		}
		/*if (min >= 50000) {
			min -= 50000;
		}
		edgeWeightMap[myPair] = min;
	}*/
	return min;
}

void Node::setConnection(Edges edge, Node& node, int criteria) {
	Node* nodePtr = &node;
	connections.push_back(nodePtr);
	edgeMap[node.city].push_back(edge);
}


Node::Node(string country,string city,string latitude,string longitude)
{
		this -> country = country;
		this -> city = city;
		this -> latitude = atof(latitude.c_str());
		this -> longitude = atof(longitude.c_str());
		this -> visited = false;
		this -> totalWeight = NULL;
		this -> duplicateInHeap = false;
		this -> transportType = "Bus";
}

Node::Node(void)
{
}

Node::~Node(void)
{
}
