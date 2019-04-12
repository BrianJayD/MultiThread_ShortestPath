#include "pthread_shortpath.h"


vector<string> visited;
map<string, int> distances;
vector<node> nodes;
pthread_mutex_t mutextotal; //mutual exclusion variable for total

int main(int argc, char const *argv[]) {
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
    //distances[nodeIndex].name = n;
    //distances[nodeIndex].path = 0;
    nodeIndex++;
  }
  infile.close();

  struct thread_data thread_data_array[nodeIndex];

  pthread_t threads[100];
  pthread_attr_t a;

  pthread_mutex_init(&mutextotal, NULL);

  pthread_attr_init(&a); //set a to all default attributes
  pthread_attr_setdetachstate(&a, PTHREAD_CREATE_JOINABLE);
  void *status;



  map<string, int>::iterator itr;

  cout << "Node being checked: " + startNode << endl;
  visited.push_back(startNode);

  node initialNode = nodes[vecIndex];

  // BRIAN
  // Prints neighbors... don't remember why I did this.... kind of got high and
  // started doing random shit
  //initialNode.paths.begin() => A leads to B, D, E

  int i = 0;
  int error;

  //for(itr = initialNode.paths.begin(); itr != initialNode.paths.end(); itr++) {
  vector<string> adjNodes;
  vector<int> adjDist;

  map<string, int>::iterator mItr;
  for(mItr = initialNode.paths.begin(); mItr != initialNode.paths.end(); mItr++) {
    adjNodes.push_back(mItr->first);
    adjDist.push_back(mItr->second);
  }

  for(int i = 0; i < initialNode.paths.size(); i++) {
    thread_data_array[i].thread_id = i;
    thread_data_array[i].key = adjNodes.at(i);
    thread_data_array[i].dist = adjDist.at(i);
    thread_data_array[i].startNode = initialNode.name;
    thread_data_array[i].d = 0;
    i++;

    error = pthread_create(&threads[i], &a, startAlgorithm,
                          (void *) &thread_data_array[i]);

    if(error){
      cout << "ERROR: create() " << error <<"\n" << endl;

    }

    cout << "END" << endl << endl;

  }

  pthread_attr_destroy(&a);


  //join threads back
  for(int x = 0; x < i; i++){
    error = pthread_join(threads[i], &status);

    if (error) {
      printf("ERROR: join() %d\n", error);
      exit(-1);
    }
  }

  pthread_mutex_destroy(&mutextotal);
  //exit main thread
  pthread_exit(NULL);


  return 0;
}

bool isVisited(string toCheck) {
  vector<string>::iterator vecItr = find(visited.begin(), visited.end(), toCheck);
  if(vecItr != visited.end()) {
    cout << toCheck << " HAS BEEN VISITED" << endl;
    return true;
  }
  return false;
}

// thread_data_array[i].thread_id = i;
// thread_data_array[i].key = adjNodes.at(i);
// thread_data_array[i].dist = adjDist.at(i);
// thread_data_array[i].startNode = initialNode.name;
// thread_data_array[i].d = 0;
void *startAlgorithm(void *threadarg){

  struct thread_data *my_data;
  my_data = (struct thread_data*) threadarg;
  pthread_mutex_lock(&mutextotal);                //aquire the lock
  updateDistances(my_data->key, my_data->dist, my_data->startNode,my_data->d);
  pthread_mutex_unlock(&mutextotal);              //release the lock
  


  // total += my_data->value;
  pthread_exit(NULL);

}

void updateDistances(string key, int dist, string startNode, int d) {
  int sum = dist + d;


  cout << "START: "<< startNode << endl << "\t";

  map<string, int>::iterator distItr = distances.find(key);
  bool noUpdate = false;

  // BRIAN
  // If node is not visited, add to distances map,
  // else check if sum is less than recorded distance and update if true.
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



  if(noUpdate) {
    return;
  }

  // // BRIAN
  // // Find visited node in vector
  // vector<node>::iterator nodeItr;
  // for(nodeItr = nodes.begin(); nodeItr != nodes.end(); nodeItr++) {
  //   string toGo = (*nodeItr).name;
  //
  //   // BRIAN
  //   // If the visited node is found in vector, go though paths of visited node
  //   // and call updateDistances again to transverse through the available paths.
  //   if(key == toGo) {
  //     cout << toGo << endl;
  //
  //     // BRIAN
  //     // Prints initial visited vector and distances map
  //     map<string, int>::iterator neighborItr;
  //
  //     // BRIAN
  //     // Go through each path and start updateDistances function again
  //     for(neighborItr = (*nodeItr).paths.begin(); neighborItr != (*nodeItr).paths.end(); neighborItr++) {
  //       string newKey = neighborItr->first;
  //       int newDist = neighborItr->second;
  //       //cout << startNode << " to " << key << endl;
  //       updateDistances(newKey, newDist, toGo, sum);
  //     }
  //   }
  // }

}

//create a function here that performs 1 iteration of bellman fords and returns a boolean
//TODO
bool bellmanFord(int numNodes, map<string, int>::iterator itr, string startNode){

  bool finished = true;

  for(int y = 0; y < numNodes; y++) {
    cout << "----------------------------------------------------------" << endl;
    cout << "NODE being checked " + nodes[y].name << endl;

    // BRIAN
    // Go through each path the node goes to

    //CHRISTIEN
    //this part will be threaded. Each thread will read each node and what they connect to
    //in parallel. Then they will sequentially update the table aftwerwards since the updated
    //depends on the ordering of all nodes being read
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

          //set our boolean value to false since there was a change in the  distance map
          if(distItr != distances.end()) {
            distItr->second = currentNodeDist + dist;
            finished = false;
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
            // change boolean to false since there was an update
            if (distItr->second > sum) {
              cout << "NODE " + key + " HAS BEEN UPDATED (current=" << distItr->second
                << " longer than " << sum << ")" << endl;
              distItr->second = sum;
              finished = false;
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
    }
  }
  return finished;
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
