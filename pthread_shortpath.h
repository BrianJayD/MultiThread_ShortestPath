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
#include <pthread.h>

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

struct thread_data {

  long thread_id;
  string key;
  int dist;
  string startNode;
  int d;

};

vector<string> visited;
map<string, int> distances;
vector<node> nodes;
pthread_mutex_t mutextotal; //mutual exclusion variable for total

bool isVisited(string toCheck);

void updateDistances(string key, int dist, string startNode, int d);

node setNode(string nodeName, string data);

int countLines(string file);

void printVisited(vector<string> vec);

void printMap(map<string, int> distMap);

bool bellmanFord(int numNodes, map<string, int>::iterator itr, string startNode);

void *startAlgorithm(void *threadarg);
