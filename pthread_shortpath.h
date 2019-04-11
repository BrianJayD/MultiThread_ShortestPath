#include <omp.h>
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <iterator>
#include <vector>
#include <algorithm>
#include <cctype>

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

node setNode(string nodeName, string data);

int countLines(string file);

void printVisited(vector<string> vec);

void printMap(map<string, int> distMap);
