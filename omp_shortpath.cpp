#include "omp_shortpath.h"

int main(int argc, char const *argv[]) {

  int numNodes = countLines(argv[1]);

  node nodes[numNodes];

  ifstream infile(argv[1]);

  string line, n, d;
  string node_delim = ":";
  int nodeIndex = 0;

  while(getline(infile, line)) {
    int delim_index = line.find(node_delim);
    n = line.substr(0, delim_index);
    d = line.substr(delim_index + 1, line.length());

    nodes[nodeIndex] = setNode(n, d);
    nodeIndex++;
  }

  infile.close();


  map<string, int>::iterator itr;

  for (int y = 0; y < numNodes; y++) {
    cout << "NODE: "<< nodes[y].name << endl;

    cout << "PATHS:" << endl;
    for (itr = nodes[y].paths.begin(); itr != nodes[y].paths.end(); itr++) {
      cout << itr->first << "\t" << itr->second << endl;
    }

  }


  return 0;
}

node setNode(string na, string data) {

  node no;
  no.name = na;

  string line;
  char delim = ',';
  stringstream ss(data);
  while(getline(ss, line, delim)) {
    char nd = '-';
    int nd_index = line.find(nd);
    string nn = line.substr(0, nd_index);
    string cost = line.substr(nd_index + 1, line.length());
    no.paths.insert(pair<string, int>(nn, stoi(cost)));

  }

  return no;

}

int countLines(string file) {

  ifstream count(file);
  string line;
  int c = 0;

  while (getline(count, line)) {
    c++;
  }

  return c;
}
