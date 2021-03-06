#include "seq_shortpath.h"

int main(int argc, char const *argv[]) {
  // Get number of nodes from file
  int numNodes = countLines(argv[1]);

  // This takes starting node as arguement and changes to upper case for comparing
  string startNode = argv[2];
  transform(startNode.begin(), startNode.end(), startNode.begin(), ::toupper);
  cout << "INPUT: " + startNode << endl;

  ifstream infile(argv[1]);

  // Variables for reading file
  string line, name, paths;
  string node_delim = ":";
  // Index of node in file and starting node in vector
  int nodeIndex = 0;
  int vecIndex = 0;

  while(getline(infile, line)) {
    int delim_index = line.find(node_delim);
    name = line.substr(0, delim_index);
    paths = line.substr(delim_index + 1, line.length());

    // Insert node in nodes vector and distances map
    nodes.push_back(setNode(name, paths));
    distances.insert(pair<string, int>(name, 0));

    // This gets index of start node in node vector
    if (startNode == name) {
      vecIndex = nodeIndex;
    }

    nodeIndex++;
  }

  // Finished reading from file
  infile.close();

  map<string, int>::iterator itr;

  cout << "Node being checked: " + startNode << endl;
  visited.push_back(startNode);
  // Put starting node at the head of visited vector
  node initialNode = nodes[vecIndex];

  // Go through initial node paths and call updateDistances for each path
  for(itr = initialNode.paths.begin(); itr != initialNode.paths.end(); itr++) {
    //cout << "-----------------START-----------------" << endl << endl;
    updateDistances(itr, initialNode.name, 0);
    //cout << "END" << endl << endl;
  }

  // Print out final vector and map
  cout << endl;
  printVisited(visited);
  cout << endl;
  printMap(distances);

  return 0;
}

/**
* Checks to see if node is visited to decide whether to update or add.
* @string toCheck: string of node being checked
* @return true: if node is already in visited vector
* @return false: if node is not found in visited vector
*/
bool isVisited(string toCheck) {
  vector<string>::iterator vecItr = find(visited.begin(), visited.end(), toCheck);
  if(vecItr != visited.end()) {
    //cout << toCheck << " HAS BEEN VISITED" << endl;
    return true;
  }
  //cout << toCheck << " HAS NOT BEEN VISITED" << endl;
  return false;
}

/*
* Tranverses through each path updating the distances map as nodes are visited
* @map::iterator neighbors: contains a map of adjacent nodes with distances
* @string startNode: node traveling from
* @int d: sum of distances from initial starting node to startNode
*/
void updateDistances(map<string, int>::iterator neighbors, string startNode, int d) {
  string key = neighbors->first;
  int dist = neighbors->second;
  // Add distance from startNode to key with cummulative distance from initial node
  int sum = dist + d;

  cout << "START: "<< startNode << endl << "\t";

  // Find key node information from distances map
  map<string, int>::iterator distItr = distances.find(key);

  // If node is not visited, add to distances map,
  // else check if sum is less than recorded distance and update if true.
  // if key has not been visited. Add to visited vector and update distances map
  // with sum value.
  if(!isVisited(key)) {
    visited.push_back(key);
    if(distItr != distances.end()) {
      cout << "ADDING " + key + " WITH DISTANCE " << sum << endl;
      distItr->second = sum;
    }
  } else {
    // if recorded distance in map is greater than sum. Update value in map.
    if(distItr->second > sum) {
      cout << "UPDATING " + key + " WITH DISTANCE " << sum << endl;
      distItr->second = sum;
    } else {
      // if recorded distance is less or equal to sum, no update needed
      cout << "NO UPDATE NEEDED (Sum=" << sum << " > Current=" <<
        distItr->second << ")"<< endl;
      return;
    }
  }

  // Find visited node in vector
  vector<node>::iterator nodeItr;
  for(nodeItr = nodes.begin(); nodeItr != nodes.end(); nodeItr++) {
    string toGo = (*nodeItr).name;

    // If the visited node is found in vector, go though paths of visited node
    // and call updateDistances again to transverse through the available paths.
    if(key == toGo) {
      cout << toGo << endl;

      // Prints initial visited vector and distances map
      /*
      cout << endl;
      printVisited(visited);
      cout << endl;
      printMap(distances);
      */

      map<string, int>::iterator neighborItr;

      // Go through each path and start updateDistances function again
      for(neighborItr = (*nodeItr).paths.begin(); neighborItr != (*nodeItr).paths.end(); neighborItr++) {
        //cout << startNode << " to " << key << endl;
        updateDistances(neighborItr, toGo, sum);
      }
    }
  }

}

// BRIAN
// Print map function
void printMap(map<string, int> distMap) {
  map<string, int>::iterator it;
  cout << "UPDATED MAP: " << endl;
  cout << "PATH\tDIST" << endl;
  for(it = distMap.begin(); it != distMap.end(); it++) {
    cout << it->first << "\t" << it->second << endl;
  }
}

void printVisited(vector<string> vec){
  cout << "PRINTNG VISITED" << endl;
  for(int i = 0; i < vec.size();i++){
    cout << vec[i] + ", ";
  }
  cout<< endl;
}

/*
* Converts values into a node by reading in data string from file.
* @string nodeName: node letter/name
* @string data: adjacent nodes with their value. (From file)
*/
node setNode(string nodeName, string data) {

  node n;
  n.name = nodeName;

  string line;
  char delim = ',';
  stringstream ss(data);
  while(getline(ss, line, delim)) {
    char pathDelim = '-';
    int nodeIndex = line.find(pathDelim);
    string neighbor = line.substr(0, nodeIndex);
    string cost = line.substr(nodeIndex + 1, line.length());
    n.paths.insert(pair<string, int>(neighbor, stoi(cost)));
  }

  return n;

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
