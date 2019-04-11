#include "pthread_shortpath.h"

int main(int argc, char const *argv[]) {
  int numNodes = countLines(argv[1]);

  // BRIAN
  // This takes starting node as arguement and changes to upper case for comparing
  string startNode = argv[2];
  transform(startNode.begin(), startNode.end(), startNode.begin(), ::toupper);
  cout << "INPUT: " + startNode << endl;

  // BRIAN
  // Changed node array to node vector
  //
  // An array of nodes; a node contains a string name and a map
  vector<node> nodes;
  map<string, int> distances;

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
    //distances[nodeIndex].name = n;
    //distances[nodeIndex].path = 0;
    nodeIndex++;
  }

  infile.close();

  map<string, int>::iterator itr;

  // BRIAN
  // Pushes start node to already visited because we start there
  //Have to create a map that will show all the nodes that we have visited so far
  vector<string> visited;
  visited.push_back(startNode);

  cout << "Node being checked: " + startNode << endl;

  node initialNode = nodes[vecIndex];

  // BRIAN
  // Prints neighbors... don't remember why I did this.... kind of got high and
  // started doing random shit
  for(itr = initialNode.paths.begin(); itr != initialNode.paths.end(); itr++) {
    string key = itr->first;            //first node connected
    int dist = itr->second;             //value of the first node connected

    cout << "PATH: " + key  + " = " << dist << endl;
    //visited.push_back(key);

  }

  // BRIAN
  // Prints initial visited vector and distances map
  cout << endl;
  printVisited(visited);
  cout << endl;
  printMap(distances);

  // BRIAN
  // Go through each node that exists
  for(int y = 0; y < numNodes; y++) {
    cout << "----------------------------------------------------------" << endl;
    cout << "NODE being checked " + nodes[y].name << endl;

    // BRIAN
    // Go through each path the node goes to
    for (itr = nodes[y].paths.begin(); itr != nodes[y].paths.end(); itr++) {
      // BRIAN
      // I got too tired to come up with short variable names.
      // Gets distance of the node we are checking from the map
      int currentNodeDist = distances.find(nodes[y].name)->second;

      string key = itr->first;            //first node connected
      int dist = itr->second;             //value of the first node connected

      // BRIAN
      // Not visited if the node being transversed from has a distance of 0
      bool notVisited = (distances.find(nodes[y].name)->second == 0);
      // BRIAN
      // Is start if startNode arguement is the node being transversed from
      bool isStart = (startNode == nodes[y].name);

      // BRIAN
      // IF, node is not the start or it is visited, update distances map.
      // ELSE, start node does not have a path to node being transversed from,
      // yet.
      if(isStart || !notVisited) {
        // BRIAN
        // If the neighboring node does not have a distance of 0 in the distances
        // map. It exists already.
        bool exists = (distances.find(key)->second != 0);

        map<string, int>::iterator distItr = distances.find(key);

        // BRIAN
        // IF, neighboring node does not exist, it is not in the map. Add to
        // visited. Set distance of neighboring node.
        // ELSE, neighboring node exists, it is in the map. Check if sum of
        // existing distance and distance to transversed from is less than the
        // distance in map. If sum is less than current distance, update map.
        if (!exists) {
          cout << "NODE " + key + " NOT FOUND" << endl;
          visited.push_back(key);

          if(distItr != distances.end()) {
            distItr->second = currentNodeDist + dist;
          }

          cout << "NODE " + key + " HAS BEEN VISITED" << endl;
        } else {
          cout << "NODE " + key + " FOUND" << endl;

          // BRIAN
          // If key is found in distances map. Get sum of current node and
          // distance to neighbor node.
          if(distItr != distances.end()) {
            int sum = currentNodeDist + dist;
            // BRIAN
            // IF, sum is less than whats in the distances map. Update.
            // ELSE, sum is too large. No update.
            if (distItr->second > sum) {
              cout << "NODE " + key + " HAS BEEN UPDATED (current=" << distItr->second
                << " longer than " << sum << ")" << endl;
              distItr->second = sum;
            } else {
              cout << "NODE " + key + " TOO FAR, NO UPDATE (current=" << distItr->second
                << " shorter than " << sum << ")" << endl;
            }
          }
        }

        printVisited(visited);
        printMap(distances);

      } else {
        // BRIAN
        // TODO
        // Needs to be a case where if the node transversing from isnt found in
        // distances map. Go through node vector again and check if any existing nodes
        // from updated distances map point to the node being transversed.
        // This is to back track because it does not work if we use C as a starting
        // node.
        cout << "CANNOT FIND PATH TO " << nodes[y].name << endl;
        //cout << "Adding " << dist << " + " << distances.find(key)->second << endl;
      }

      // BRIAN
      // COMMENTED OUT CHRISTIENS CODE
      //If we have not already visited the node then add it to our visited list
      /*if(notVisited){
        cout << "NOT FOUND" << endl;
        visited.push_back(key);
        cout << "Placed " + key << endl;
        //new distance = parent node distance plus distance to travel to new node
        map<string, int>::iterator posIter = distances.find(key);
        if(posIter != distances.end()){
          //cout << "Distance to " + key + ": "<< dist + distances.find(nodes[y].name)->second << endl;
          posIter->second = dist + distances.find(nodes[y].name)->second;
        }


        //distances.insert(pair<string, int>(key, dist));

      }else{
        cout << "FOUND" << endl;
        //Now we have to check if this new path to the already visited variable is an improvement
        if(distances.find(key)->second > dist + distances.find(nodes[y].name)->second && !notVisited ){
          cout <<  "new distance to " + key + ": " << dist  << " + " << distances.find(nodes[y].name)->second << endl;
        }
        //otherwise we update if we see an improvement
        //get the current value of item we are trying to update
      }*/
      //cout << itr->first << "\t" << itr->second << endl;
    }
    //printVisited(visited);
    //itterate through each possible path connected to the node we are looking at
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
