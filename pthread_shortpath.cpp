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

    error = pthread_create(&threads[i], &a, startAlgorithm,
                          (void *) &thread_data_array[i]);

    if(error){
      cout << "ERROR: create() " << error <<"\n" << endl;

    }

    cout << "END" << endl << endl;

  }

  pthread_attr_destroy(&a);


  //join threads back
  for(int i = 0; i < initialNode.paths.size(); i++){
    error = pthread_join(threads[i], NULL);

    if (error) {
      printf("ERROR: join() %d\n", error);
      exit(-1);
    }
  }

  printMap(distances);

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


void *startAlgorithm(void *threadarg){

  struct thread_data *my_data;
  my_data = (struct thread_data*) threadarg;
  updateDistances(my_data->key, my_data->dist, my_data->startNode,my_data->d);

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
  pthread_mutex_lock(&mutextotal);                //aquire the lock
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
  pthread_mutex_unlock(&mutextotal);              //release the lock



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
