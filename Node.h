#pragma once
#include <string>
#include <vector>
#include <map>
#include "Edges.h"

using namespace std;

class Node
{
public:
	//map of edges, with city destination as key
	map<string, vector<Edges> > edgeMap;
	map<pair<string,int>, double> edgeWeightMap;
	double totalWeight;
	bool duplicateInHeap;
	bool visited;
	string country, city;
	double latitude, longitude;
	vector<Node*> connections;
	string transportType;
	string getTransportType(string city);
	double edgeWeight(string city, int criteria);
	void printConnections();
	void setConnection(Edges edge, Node& node, int criteria);
	string getCountry();
	Node(string country,string city,string latitude,string longitude);
	Node(void);
	~Node(void);

	 bool operator <(const Node &b)const
	 {
		 return totalWeight > b.totalWeight;
	 }

};

