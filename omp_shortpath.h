#include <omp.h>
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <string>

using namespace std;

struct node {
  string name;
};

void getNeighbors(string data);
