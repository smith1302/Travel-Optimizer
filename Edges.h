#pragma once

#include <string>

using std::string;

class Edges
{
public:
	string origin, destination, transport, time, cost, notes;
	double getWeight(int criteria);
	Edges(string origin,string destination,string transport,string time,string cost,string notes);
	Edges(void);
	~Edges(void);
};

