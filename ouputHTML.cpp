#include "stdafx.h"
#include "ouputHTML.h"
#include <iostream>
#include <fstream>
using namespace std;


ouputHTML::ouputHTML(stack<Node> path, int criteria, string start, string end, string outputFile)
{
	int numMarkerPairs = path.size() - 1;
	string criteriaString = "Fastest";
	double totalTime = 0;
	double totalCost = 0;
	switch (criteria) {
	case 1:
		criteriaString = "Fastest";
		break;
	case 2:
		criteriaString = "Cheapest";
		break;
	}

	ofstream write;
	write.open(outputFile);
	stringstream header;
	header << "<HTML><HEAD><TITLE>" << criteriaString << " path from "<< start <<" to "<< end << "</TITLE></HEAD>"
	"<script type=\"text/javascript\" src=\"http://maps.google.com/maps/api/js?sensor=false\"></script>"
	"<script>function initialize() { "
	"var myOptions = { zoom: 3, center: new google.maps.LatLng(0, 0), mapTypeId: google.maps.MapTypeId.ROADMAP};"
	"var map=new google.maps.Map(document.getElementById(\"map\"), myOptions);";
	write << header.str();
	Node previous = path.top(); path.pop();
	for(int i = 0; i < numMarkerPairs; i++) {
		string city1 = previous.city;
		string country1 = previous.country;
		double latitude1 = previous.latitude;
		double longitude1 = previous.longitude;
		// ==================================
		Node node = path.top(); path.pop();
		string city2 = node.city;
		string country2 = node.country;
		double latitude2 = node.latitude;
		double longitude2 = node.longitude;

		double cost = previous.edgeWeight(city2,2);
		double time = previous.edgeWeight(city2,1);
		string transport = previous.getTransportType(city2);
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

		previous = node;

	}

	string footer = 
	"} google.maps.event.addDomListener(window, \"load\", initialize);"
	"</script></HEAD><BODY><div id=\"map\" style=\"width:100%;height:90%;\"></div></BODY></HTML>";
	write << footer;
	
	write.close();

}


ouputHTML::~ouputHTML(void)
{
}
