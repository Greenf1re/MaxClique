#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#define DEBUG false
using namespace std;
int NODES = 0;
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
        // ==
        bool operator==(const Node& n) const{
            return id == n.id;
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
void ReadGraph(string filename, uint8_t **graph, vector<Node>& nodeList){
    ifstream file;
    file.open(filename);
    if(file.is_open()){
        for(int i = 0; i < NODES; i++){
            file >> nodeList[i];
            for(int j = 0; j <= i; j++){
                if (i == j) graph[i][j] = '0';
                else{
                    // uint8_t x = -1;
                    // file >> x;
                    file >> graph[i][j];
                    // graph[i][j] = x;
                    graph[j][i] = graph[i][j];
                }
            }
        }
    }
}
void CountEdges(uint8_t** Graph, vector<Node>& clique){
    // Count the number of edges in the graph and store in return vector
    for(int i = 0; i < (int)clique.size(); i++){
        for(int j = 0; j < (int)clique.size(); j++){
            if((uint8_t)Graph[clique[i].name()][clique[j].name()] != '0'){  // If there is an edge
                clique[i].degree++; // Increment the degree of the node
            }
        }
    }
}
int AverageDegree(vector<Node>& nodeList){
    double sum = 0;
    for(int i = 0; i < NODES; i++){
        sum += nodeList[i].degree;  // Sum the degrees of all nodes
    }
    return round(sum/(double)NODES);
}
void PrintGraph(uint8_t **graph, vector<Node>& nodeList){
    for(int i = 0; i < NODES; i++){
        cout << nodeList[i] << " ";
        for(int j = 0; j < NODES; j++){ // Print the graph
            cout << graph[i][j] << " ";
        }
        cout << endl;
    }
}
int FindAbsSimilarity(uint8_t **graph, uint8_t *row, int compareRow){ // Find the similarity between a row and the graph using index
    int count = 0;
    for(int i = 0; i < NODES; i++){
        if(graph[compareRow][i] == row[i]){ //Absolute similarity instead of just 1s
            count++;
        }
    }
    return count;
}
int FindPosSimilarity(uint8_t **graph, uint8_t *row, int compareRow){   // Find the similarity between a row and the graph using index
    int count = 0;
    for(int i = 0; i < NODES; i++){
        if(graph[compareRow][i] == row[i] && row[i] != '0'){ //Absolute similarity instead of just 1s
            count++;
        }
    }
    return count;
}
int CompareAbsSimilarity(uint8_t *row1, uint8_t* row2){ // Compare two rows for absolute similarity
    int count = 0;
    for(int i = 0; i < NODES; i++){
        if(row1[i] == row2[i]){ //Absolute similarity instead of just 1s
            count++;
        }
    }
    return count;
}
bool VerifyClique(uint8_t **graph, vector<Node>& clique){
    for(int i = 0; i < (int)clique.size(); i++){
        for(int j = i+1; j < (int)clique.size(); j++){
            if(graph[clique[i].name()][clique[j].name()] == '0'){   // If there is no edge between the nodes
                return false;
            }
        }
    }
    return true;
}
void SaveCliqueToFile(vector<Node>& clique){
    ofstream file;
    string filename;
    // Filename is NODES + cliqueSize
    filename = ".\\Solutions\\Size" + to_string(NODES) + "\\s" + to_string((int)clique.size()) + "_aaaguilar1.sol";
    file.open(filename);
    if(file.is_open()){
        for(int i = 0; i < (int)clique.size(); i++){
            file << clique[i].name() << " ";
        }
        file << endl;
    }
    else cout << "Error opening file: " << filename << endl;
}
int main(int argc, char** argv){
    uint8_t **graph = NULL;
    vector<Node> nodeList;
    string filename;
    string solutionFilename;
    if(argc < 4){
        cout << "CLI Usage: " << argv[0] << " <graph file> <Solution> <count>" << endl;
        // return 1;
        cout << "Enter the number of nodes: ";
        cin >> NODES;
        cout << "Enter the graph filename: ";
        cin >> filename;
        cout << "Enter the solution to verify:";
        cin >> solutionFilename;
    }
    else{
        NODES = atoi(argv[3]);
        filename = argv[1];
        solutionFilename = argv[2];
    }
    graph = new uint8_t*[NODES];    // Allocate memory for the graph
    for(int i = 0; i < NODES; i++){
        graph[i] = new uint8_t[NODES];
    }
    nodeList.resize(NODES);

    ReadGraph(filename, graph, nodeList);   // Read the graph
    // print the graph
    if(DEBUG){
        PrintGraph(graph, nodeList);
    }
    // Read the solution file
    ifstream file;
    file.open(solutionFilename);
    vector<Node> clique;    // Store the clique
    if(file.is_open()){
        int node;
        while(file >> node){
            clique.push_back(Node(node));   // Add the node to the clique
        }
    }
    else{
        cout << "Error opening file: " << solutionFilename << endl; 
        return 1;
    }
    // Verify the clique
    if(VerifyClique(graph, clique)){    // If the clique is valid
        cout << "Clique is valid" << endl;
    }
    else{
        cout << "Clique is invalid" << endl;
    }
    ////////////////////////////////////////////////////////////////////
    // Find density of the graph
   
    return 0;
}