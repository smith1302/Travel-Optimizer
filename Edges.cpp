#include "stdafx.h"
#include "Edges.h"

double Edges::getWeight(int criteria) {
	double returner;
	if (criteria == 1) {
		 returner = atof(this->time.c_str());
	}else if (criteria == 2) {
		returner = atof(this->cost.c_str());
	}
	if (transport == "plane") {
		returner *= 500;
	}
	return returner;
}

Edges::Edges(string origin,string destination,string transport,string time,string cost,string notes)
{
	this -> origin = origin;
	this -> destination = destination;
	this -> transport = transport;
	this -> time = time;
	this -> cost = cost;
	this -> notes = notes;
}

Edges::Edges(void)
{
}


Edges::~Edges(void)
{
}
