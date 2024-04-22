#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#define DEBUG true
using namespace std;
int NODES = 0;
// A node contains the id (name of the node) and its degree (number of edges connected to it)
class Node{
    public:
        int id;
        int degree;
        Node(int id, int degree){
            this->id = id;
            this->degree = degree;
        }
        Node(){
            id = 0;
            degree = 0;
        }
        Node(int u){
            id = u;
            degree = 0;
        }
        bool operator<(const Node& n) const{
            return degree > n.degree;
        }
        // overload << operator
        friend ostream& operator<<(ostream& os, const Node& n){
            os << n.id;
            return os;
        }
        // overload [] operator
        int operator[](int index){
            if(index == 0) return id;
            else return degree;
        }
        // Overload >> operator
        friend istream& operator>>(istream& is, Node& n){
            is >> n.id;
            return is;
        }
        int name(){
            return id;
        }
};
void ReadGraph(string filename, int **graph, vector<Node>& nodeList){   // Read the graph from the file
    ifstream file;
    file.open(filename);
    if(file.is_open()){
        for(int i = 0; i < NODES; i++){
            file >> nodeList[i];
            for(int j = 0; j <= i; j++){
                if (i == j) graph[i][j] = 1;
                else{
                    file >> graph[i][j];
                    graph[j][i] = graph[i][j];
                }
            }
        }
    }
}
void CountEdges(int** Graph, vector<Node>& clique){ // Count the number of edges in the graph and store in return vector
    // Count the number of edges in the graph and store in return vector
    for(int i = 0; i < (int)clique.size(); i++){
        for(int j = 0; j < (int)clique.size(); j++){
            if(Graph[clique[i].name()][clique[j].name()] == 1){ 
                // count++;
                clique[i].degree++;
            }
        }
    }
}
int AverageDegree(vector<Node>& nodeList){  // Calculate the average degree of the nodes in the graph
    int sum = 0;
    for(int i = 0; i < NODES; i++){
        sum += nodeList[i].degree;
    }
    return sum/NODES;
}
void PrintGraph(int **graph, vector<Node>& nodeList){   // Print the graph
    for(int i = 0; i < NODES; i++){
        cout << nodeList[i] << " ";
        for(int j = 0; j < NODES; j++){
            cout << graph[i][j] << " ";
        }
        cout << endl;
    }
}
bool VerifyClique(int **graph, vector<Node>& clique){   // Verify if the nodes in the clique are connected
    for(int i = 0; i < (int)clique.size(); i++){
        for(int j = i+1; j < (int)clique.size(); j++){
            if(graph[clique[i].name()][clique[j].name()] == 0){
                return false;
            }
        }
    }
    return true;
}
int main(int argc, char** argv){
    int **graph = NULL;
    vector<Node> nodeList;
    string filename;

    if(argc < 3){
        cout << "CLI Usage: " << argv[0] << " <graph file> <count>" << endl;
        // return 1;
        cout << "Enter the number of nodes: ";
        cin >> NODES;
        cout << "Enter the filename: ";
        cin >> filename;
    }
    else{
        NODES = atoi(argv[2]);
        filename = argv[1];
    }
    graph = new int*[NODES];    // Allocate memory for the graph
    for(int i = 0; i < NODES; i++){
        graph[i] = new int[NODES];
    }
    nodeList.resize(NODES);

    ReadGraph(filename, graph, nodeList);   // Read the graph from the file
    // print the graph
    if(DEBUG){
        PrintGraph(graph, nodeList);
    }
    ////////////////////////////////////////////////////////////////////
    vector<Node> maxClique;
    if(DEBUG){
        CountEdges(graph, nodeList);
        for (int i = 0; i < NODES; i++){
            cout << nodeList[i] << " " << nodeList[i].degree << endl;
        }
        // sort(nodeList.begin(), nodeList.end());
        int avgDegree = AverageDegree(nodeList);
        cout << "Average Degree: " << avgDegree << endl;
    }

    // Bruteforce using combinations of nodes
    int maxSize = 0;
    for (int size = 1; size <= NODES; size++) { // Iterate over all possible sizes of the clique
        vector<bool> v(NODES);                  // Create a vector of booleans of size NODES to represent the combination
        fill(v.end() - size, v.end(), true);  // Initialize the combination vector
        do {
            vector<Node> currentClique; 
            for (int i = 0; i < NODES; i++) {
                if (v[i]) {                      // If the node is in the combination
                    currentClique.push_back(i); // Add the node to the current clique
                }
            }
            if (VerifyClique(graph, currentClique)) {
                if ((int)currentClique.size() > maxSize) {
                    maxSize = currentClique.size();
                    maxClique = currentClique;
                    cout << "New Size: " << maxSize << endl;
                }
            }
        } while (next_permutation(v.begin(), v.end())); // Generate the next combination
    }
    
    ////////////////////////////////////////////////////////////////////
    cout << "Max Clique: ";
    for(int i = 0; i < (int)maxClique.size(); i++){
        cout << nodeList[maxClique[i].name()] << " ";
    }
    cout << endl << "Node degrees: ";
    for(int i = 0; i < (int)maxClique.size(); i++){
        cout << nodeList[maxClique[i].name()].degree << " ";
    }
    cout << endl;
    cout << "Size: " << maxClique.size() << endl;
    return 0;
}