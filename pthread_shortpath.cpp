#include "pthread_shortpath.h"

int main(int argc, char const *argv[]) {
  int numNodes = countLines(argv[1]);

  //An array of nodes; a node contains a string name and a map
  node nodes[numNodes];
  Distance distances[numNodes];

  ifstream infile(argv[1]);

  string line, n, d;
  string node_delim = ":";
  int nodeIndex = 0;

  while(getline(infile, line)) {
    int delim_index = line.find(node_delim);
    n = line.substr(0, delim_index);
    d = line.substr(delim_index + 1, line.length());

    nodes[nodeIndex] = setNode(n, d);
    distances[nodeIndex].name = n;
    distances[nodeIndex].path = 0;
    nodeIndex++;
  }

  infile.close();


  map<string, int>::iterator itr;


  // cout <<"Before first iteration of Bellman's" <<endl;
  // for(int i= 0; i < numNodes; i++){
  //   cout << "Node: " + distances[i].name + " distance: " <<  distances[i].path  << endl;
  // }

  //Have to create a map that will show all the nodes that we have visited so far
  vector<string> visited;

  for (int y = 0; y < numNodes; y++) {

    cout << "NODE: "<< nodes[y].name << endl;

    //cout << "PATHS:" << endl;
    for (itr = nodes[y].paths.begin(); itr != nodes[y].paths.end(); itr++) {
      string key = itr->first;

      //Check if the node we are inspecting has been visited yet.
      //TODO

      //If we have not already visited the node then add it to our visited list
      if(find(visited.begin(), visited.end(), key) == visited.end()){
        visited.push_back(itr->first);
        cout << itr->first << endl;
      }

      //cout << itr->first << "\t" << itr->second << endl;
    }

  }





  //Here we have to go through each node for a certain amount of iterations
  /*while(some boolean == false){

    // read through each node and keep its count



    }
  */

  return 0;
}


//create a function here that performs 1 iteration of bellman fords and returns a boolean
//TODO
//boolean iterations()


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

/*
Get the line count of the file we are reading to get the number of nodes
@String file:file name
@return c: integer of the amount of nodes in the algorithm
*/
int countLines(string file) {

  ifstream count(file);
  string line;
  int c = 0;

  while (getline(count, line)) {
    c++;
  }

  return c;
}
