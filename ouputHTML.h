#pragma once
#include <string>
#include <stack>
#include <map>
#include <sstream>
#include "Node.h"

using namespace std;

class ouputHTML
{
public:
	string output;
	ouputHTML(stack<Node> path, int criteria, string start, string end, string outputFile);
	~ouputHTML(void);
};

