#include <omp.h>
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <iterator>

using namespace std;

struct node {
  string name;
  map<string, int> paths;
};


node setNode(string na, string data);

int countLines(string file);
