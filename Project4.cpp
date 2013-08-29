// Project4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Node.h"
#include "Edges.h"
#include <sstream>
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <stack>
#include <queue>
#include <functional>

#include <cstdio>
#include <ctime>

using namespace std;
using std::getline;

/*
struct node_comparison 
{
    bool operator()(const Node& a, const Node& b) const 
    {
		return a.totalWeight > b.totalWeight;
    }
};*/

template <class T, class F>
class reservable_priority_queue: public std::priority_queue<T, F>
{
public:
    typedef typename std::priority_queue<T, F>::size_type size_type;
    reservable_priority_queue(size_type capacity = 0) { reserve(capacity); };
    void reserve(size_type capacity) { this->c.reserve(capacity); } 
    size_type capacity() const { return this->c.capacity(); } 
};



int main(int argc, char* argv[])
{
	string start, end, criteria;
	string startCity = "";
	string endCity = "";
	string outputFile, citiesFile, routesFile;
	if (argc > 1) {
		start = argv[4];
		end = argv[5];
		criteria = argv[6];
		if (criteria.compare("cheapest") == 0) {
			criteria = "2";
		}else{
			criteria = "1";
		}
		outputFile = argv[3];
		routesFile = argv[2];
		citiesFile = argv[1];
	}else{
		cout << "~ Capital to Captial. All in one travel calculator 1.0 ~" << endl << endl;
		cout << "Starting Country: ";
		getline(cin,start);
		cout << "Destination Country: ";
		getline(cin,end);
		cout << "Choose a criteria: \n1. Fastest Route\n2. Cheapest Route" << endl;
		cin >> criteria;
		cout << "Output file name (end with .html)" << endl;
		cin >> outputFile;
		cout << endl;
		citiesFile = "cities.csv";
		routesFile = "routes.csv";
	}

	stack<Node> path;
	stack<Node> unresolvedNeighbors;
	map<string, Node> nodeMap;
	map<string, Node*> previous;
	map<string, double> dist;


	//Country Name, City Name, Latitude, Longitude
	ifstream nodes;
	nodes.open(citiesFile.c_str());
	string country, city, latitude, longitude;
	bool newCountry = true;
	while(nodes.good()) {
		getline(nodes,country,',');
		getline(nodes,city,',');
		getline(nodes,latitude,',');
		getline(nodes,longitude);
		Node node(country, city, latitude, longitude);
		nodeMap[city] = node;
		dist[city] = 999999;
		previous[city] = NULL;
		if (country == start) {
			startCity = city;
		}
		if (country == end) {
			endCity = city;
		}
	}

	//Origin City, Destination City, Type of Transport, Average time required, Average cost USD, "Notes (these may include the CSV delimiter)"
	ifstream edges;
	edges.open(routesFile.c_str());
	string origin, destination, transport, time, cost, notes;
	int numEdges = 0;
	while(edges.good()) {
		numEdges++;
		getline(edges, origin, ',');
		getline(edges, destination, ',');
		getline(edges, transport, ',');
		getline(edges, time, ',');
		getline(edges, cost, ',');
		getline(edges, notes);
		//if country node exists
		if (nodeMap.find(origin) != nodeMap.end() && nodeMap.find(destination) != nodeMap.end()) {
			//for each city, add where it connects to
			nodeMap[origin].setConnection(Edges(origin,destination,transport,time,cost,notes), nodeMap[destination], atoi(criteria.c_str()));
		}
	}

	reservable_priority_queue<Node, vector<Node> > minHeap;
	minHeap.reserve(numEdges);

		//add neighbors of start node, assuming not visited of course
		int intCriteria = atoi(criteria.c_str());
		Node& startNode = nodeMap[startCity];
		previous[startCity] = NULL;
		startNode.totalWeight = 0;
		minHeap.push(startNode);
		dist[startCity] = 0;
		Node* currentAdjNode;
		string adjCity;

		double edgeWeight;
		double sourceWeight;
		double weight;

		clock_t startTime = clock();
		while(!minHeap.empty()) {
			//dist[city] holds shortest path for that city
			//if current.totalWeight does not match, then it's not the newest one. skip.

			while (minHeap.top().totalWeight != dist[minHeap.top().city]) {
				minHeap.pop();
			}
			Node nt = minHeap.top();
			double currentCityDis = dist[minHeap.top().city];
			nt.visited = true;
			if ((currentCityDis == 999999 && currentCityDis != 0) || nt.city == endCity) {
				break;
			}
			//vector<Node*> &adjNodes = nt.connections;
			//calculate total weight from source to node
			for (unsigned int i  = 0; i < nt.connections.size(); i++) {
				currentAdjNode = nt.connections.at(i);
				adjCity = currentAdjNode->city;
				if (!currentAdjNode->visited && nt.city != adjCity) {
					edgeWeight = nt.edgeWeight(adjCity, intCriteria);
					sourceWeight = dist[nt.city];
					weight = sourceWeight + edgeWeight;
					//if the total weight < the total weight set at that country OR there is weight set
					if (weight < dist[adjCity]) {
						//set dist of node from start to weight
						dist[adjCity] = weight;
						//set previous node of current city
						previous[adjCity] = &nodeMap[nt.city];
						//create copy and push into heap
						currentAdjNode->totalWeight = weight;
						minHeap.push(*currentAdjNode);
					}
				}
			}
			minHeap.pop();
		}

	clock_t endTime = clock();
	clock_t clockTicksTaken = endTime - startTime;
	double timeInSeconds = clockTicksTaken / (double) CLOCKS_PER_SEC;

	Node* current = &nodeMap[endCity];
	double weightCost = dist[current->city];
	while(previous[current->city] != NULL && current->city != startCity) {
		path.push(*current);
		current = previous[current->city];
	}
	path.push(*current);

	//ouputHTML o(path, atoi(criteria.c_str()), start, end, outputFile);

	/*
	int i = 0;
	while(!path.empty()) {
		for (int j = 0; j <= i; j++) {
			cout << ">>";
		}
		cout << " " << path.top().country << "(" << path.top().city << ")" << endl;
		path.pop();
		i++;
	}
	if (criteria == "1") {
		cout << "Time Taken: " << weightCost << " hours" << endl;
	}else{
		cout << "Total Cost: " << weightCost << " USD" << endl;
	}
	string test;
	cin >> test;
	*/

	int numMarkerPairs = path.size() - 1;
	string criteriaString = "Fastest";
	double totalTime = 0;
	double totalCost = 0;
	switch (atoi(criteria.c_str())) {
	case 1:
		criteriaString = "Fastest";
		break;
	case 2:
		criteriaString = "Cheapest";
		break;
	}

	ofstream write;
	write.open(outputFile.c_str());
	stringstream header;
	header << "<HTML><HEAD><TITLE>" << criteriaString << " path from "<< start <<" to "<< end << "</TITLE></HEAD>"
	"<script type=\"text/javascript\" src=\"http://maps.google.com/maps/api/js?sensor=false\"></script>"
	"<script>function initialize() { "
	"var myOptions = { zoom: 3, center: new google.maps.LatLng(0, 0), mapTypeId: google.maps.MapTypeId.ROADMAP};"
	"var map=new google.maps.Map(document.getElementById(\"map\"), myOptions);";
	write << header.str();
	Node p = path.top(); 
	path.pop();
	for(int i = 0; i < numMarkerPairs; i++) {
		string city1 = path.top().city;
		string country1 = p.country;
		double latitude1 = p.latitude;
		double longitude1 = p.longitude;
		// ==================================
		Node node = path.top(); path.pop();
		string city2 = node.city;
		string country2 = node.country;
		double latitude2 = node.latitude;
		double longitude2 = node.longitude;

		double cost = p.edgeWeight(city2,2);
		double time = p.edgeWeight(city2,1);
		string transport = p.getTransportType(city2);
		totalTime += time;
		totalCost += cost;
		if (transport == "plane") {
			cost /= 500;
			time /= 500;
		}
		stringstream markers;
		markers <<
		"var marker" << i << " = new google.maps.Marker({ position: new google.maps.LatLng(" << latitude1 << ", " << longitude1 << "), map: map, title:\""<< city1 <<", "<< country1 <<"\"});"
		"var marker" << (i+1) << " = new google.maps.Marker({ position: new google.maps.LatLng(" << latitude2 << ", " << longitude2 << "), map: map, title:\""<< city2 <<", "<< country2 <<"\"});";
		write << markers.str();

		stringstream set;
		set << "var contentString"<< i <<" = \""<< city1 <<", "<< country1 <<" --> "<< city2 <<", "<< country2 <<" ("<< transport <<" - "<< time <<" hours - $"<< cost <<")\";"
		"var path"<< i <<" = new google.maps.Polyline({ path: [new google.maps.LatLng(" << latitude1 << ", " << longitude1 << "), new google.maps.LatLng(" << latitude2 << ", " << longitude2 << ")], strokeColor: \"#0000FF\", strokeOpacity: 1.0, strokeWeight: 2}); "
		"path"<< i <<".setMap(map); "
		"google.maps.event.addListener(path"<< i <<", \"click\", function(event) { alert(contentString"<< i <<"); });";
		write << set.str();

		p = node;

	}

	string footer = 
	"} google.maps.event.addDomListener(window, \"load\", initialize);"
	"</script></HEAD><BODY><div id=\"map\" style=\"width:100%;height:90%;\"></div></BODY></HTML>";
	write << footer;
	
	write.close();



	return 0;
}

