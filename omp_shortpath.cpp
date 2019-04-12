#include "omp_shortpath.h"

vector<string> visited;
map<string, int> distances;
vector<node> nodes;


int main(int argc, char const *argv[]) {
  omp_set_num_threads(NUM_THREADS);
  int num_threads = omp_get_num_threads();

  #pragma omp parallel for
  for (int i = 0; i < NUM_THREADS; i++) {
    #pragma omp critical
    {
      int thread_id = omp_get_thread_num();
      cout << "THREAD ID: " << thread_id << endl;
    }
  }

  int numNodes = countLines(argv[1]);

  // BRIAN
  // This takes starting node as arguement and changes to upper case for comparing
  string startNode = argv[2];
  transform(startNode.begin(), startNode.end(), startNode.begin(), ::toupper);
  cout << "INPUT: " + startNode << endl;

  ifstream infile(argv[1]);

  string line, name, paths;
  string node_delim = ":";
  int nodeIndex = 0;
  int vecIndex = 0;

  while(getline(infile, line)) {
    int delim_index = line.find(node_delim);
    name = line.substr(0, delim_index);
    paths = line.substr(delim_index + 1, line.length());

    nodes.push_back(setNode(name, paths));
    distances.insert(pair<string, int>(name, 0));

    // BRIAN
    // This gets index of start node in node vector
    if (startNode == name) {
      vecIndex = nodeIndex;
    }

    nodeIndex++;
  }

  infile.close();

  cout << "Node being checked: " + startNode << endl;
  visited.push_back(startNode);

  node initialNode = nodes[vecIndex];

  // BRIAN
  // Go through initial node paths and call updateDistances for each path
  /*
  for(itr = initialNode.paths.begin(); itr != initialNode.paths.end(); itr++) {
    cout << "-----------------START-----------------" << endl << endl;
    updateDistances(itr, initialNode.name, 0);
    cout << "-----------------END-----------------" << endl << endl;
  }
  */

  vector<string> adjNodes;
  vector<int> adjDist;

  map<string, int>::iterator mItr;
  for(mItr = initialNode.paths.begin(); mItr != initialNode.paths.end(); mItr++) {
    adjNodes.push_back(mItr->first);
    adjDist.push_back(mItr->second);
  }

  #pragma omp parallel for shared(initialNode, adjNodes, adjDist)
  for(int i = 0; i < initialNode.paths.size(); i++) {
    //string nodeStr = initialNode.paths.at(i);
    /*
    #pragma omp critical
    {
      cout << "THREAD ID  = " << omp_get_thread_num() << endl;
      cout << "WORKING ON " << adjNodes.at(i) << "\t" << adjDist.at(i) << endl;
    }*/

    cout << "-----------------START-----------------" << endl << endl;
    updateDistances(adjNodes.at(i), adjDist.at(i), initialNode.name, 0);
    cout << "-----------------END-----------------" << endl << endl;

  }

  cout << endl;
  printVisited(visited);
  cout << endl;
  printMap(distances);


  return 0;
}

// BRIAN
// Checks to see if node is visited to decide whether to update or add
bool isVisited(string toCheck) {
  vector<string>::iterator vecItr = find(visited.begin(), visited.end(), toCheck);
  if(vecItr != visited.end()) {
    //cout << toCheck << " HAS BEEN VISITED" << endl;
    return true;
  }
  //cout << toCheck << " HAS NOT BEEN VISITED" << endl;
  return false;
}

// Brian
// Tranverses through each path updating the distances map as nodes are visited
void updateDistances(string key, int dist, string startNode, int d) {
  int sum = dist + d;

  cout << "START: "<< startNode << endl << "\t";

  map<string, int>::iterator distItr = distances.find(key);
  bool noUpdate = false;

  // BRIAN
  // If node is not visited, add to distances map,
  // else check if sum is less than recorded distance and update if true.
  #pragma omp critical
  {
    if(!isVisited(key)) {
      visited.push_back(key);
      if(distItr != distances.end()) {
        cout << "ADDING " + key + " WITH DISTANCE " << sum << endl;
        distItr->second = sum;

      }
    } else {
      if(distItr->second > sum) {
        cout << "UPDATING " + key + " WITH DISTANCE " << sum << endl;
        distItr->second = sum;
      } else {
        cout << "NO UPDATE NEEDED (Sum=" << sum << " > Current=" <<
        distItr->second << ")"<< endl;
        noUpdate = true;
      }
    }
  }

  if(noUpdate) {
    return;
  }

  // BRIAN
  // Find visited node in vector
  vector<node>::iterator nodeItr;
  for(nodeItr = nodes.begin(); nodeItr != nodes.end(); nodeItr++) {
    string toGo = (*nodeItr).name;

    // BRIAN
    // If the visited node is found in vector, go though paths of visited node
    // and call updateDistances again to transverse through the available paths.
    if(key == toGo) {
      cout << toGo << endl;

      // BRIAN
      // Prints initial visited vector and distances map
      /*
      cout << endl;
      printVisited(visited);
      cout << endl;
      printMap(distances);
      */
      map<string, int>::iterator neighborItr;

      // BRIAN
      // Go through each path and start updateDistances function again
      for(neighborItr = (*nodeItr).paths.begin(); neighborItr != (*nodeItr).paths.end(); neighborItr++) {
        string newKey = neighborItr->first;
        int newDist = neighborItr->second;
        //cout << startNode << " to " << key << endl;
        updateDistances(newKey, newDist, toGo, sum);
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
