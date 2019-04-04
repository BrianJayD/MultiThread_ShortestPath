#include "omp_shortpath.h"

int main(int argc, char const *argv[]) {

  ifstream infile(argv[1]);

  string line, n, d;
  string node_delim = ":";

  while(getline(infile, line)) {
    int delim_index = line.find(node_delim);
    n = line.substr(0, delim_index);
    d = line.substr(delim_index + 1, line.length());

    cout << "NODE " << n << ":" << endl;

    getNeighbors(d);

  }
  infile.close();

  return 0;
}

void getNeighbors(string data) {
  //cout << data << endl << endl;
  string line;
  char delim = ',';
  stringstream ss(data);

  cout << "Dest.\tCost" << endl;

  while(getline(ss, line, delim)) {
    char nd = '-';
    int nd_index = line.find(nd);
    string nn = line.substr(0, nd_index);
    string cost = line.substr(nd_index + 1, line.length());

    cout << nn << "\t" << cost << endl;

  }
  cout << endl;

}
