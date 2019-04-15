#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <iterator>
#include <vector>
#include <algorithm>
#include <cctype>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

struct node {
  string name;
  map<string, int> paths;
};

//this will hold the shortest path for each individual node
struct Distance {
  string name;
  int path;
};

vector<string> visited;
map<string, int> distances;
vector<node> nodes;

bool isVisited(string toCheck);

void updateDistances(map<string, int>::iterator neighbors, string startNode, int d);

node setNode(string nodeName, string data);

int countLines(string file);

void printVisited(vector<string> vec);

void printMap(map<string, int> distMap);
