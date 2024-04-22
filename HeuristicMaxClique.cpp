#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <chrono>
#define DEBUG false
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
            return degree > n.degree; // Is actually greater than, but I didn't want to mess with the sort function
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
// An edge contains two nodes, u and v. Methods include getting the nodes, getting the name of the nodes, and comparing edges
class Edge{
    Node u;
    Node v;
    public:
        Edge(Node u, Node v){
            this->u = u;
            this->v = v;
        }
        Node getU(){
            return u;
        }
        Node getV(){
            return v;
        }
        int uName(){
            return u.name();
        }
        int vName(){
            return v.name();
        }
        bool operator==(const Edge& e) const{
            return (u == e.u && v == e.v) || (u == e.v && v == e.u);
        }
        bool operator<(Edge& e){
            // return u < e.u;
            // Find max of u and v
            // int max1 = max(u.name(), v.name());
            // int max2 = max(e.uName(), e.vName());
            int max1 = u.degree + v.degree;
            int max2 = e.u.degree + e.v.degree;
            return max1 < max2; // Compares the sum of the degrees of the nodes
        }
        int degree(){
            // return max(u.degree, v.degree);
            return u.degree + v.degree;
        }
        friend ostream& operator<<(ostream& os, const Edge& e){
            os << e.u << "-" << e.v;
            return os;
        }
};
void CountInvEdges(uint8_t** Graph, vector<Node>& clique){ // Inverse of CountEdges. Counts non-connections
    // Count the number of edges in the graph and store in return vector
    for(int i = 0; i < (int)clique.size(); i++){
        for(int j = 0; j < i; j++){
            if((uint8_t)Graph[clique[i].name()][clique[j].name()] == '0'){ 
                clique[i].degree++;
                clique[j].degree++;
                // cout << "Incrementing: " << clique[i] << " At x,y " << i << " " << j << endl;
                // cout << "Incrementing: " << clique[j] << " At x,y " << i << " " << j << endl << endl;
            }
        }
    }
}
void CountEdges(uint8_t** Graph, vector<Node>& clique){ //Counts connections in the graph
    // Count the number of edges in the graph and store in return vector
    for(int i = 0; i < (int)clique.size(); i++){
        for(int j = 0; j < (int)clique.size(); j++){
            if((uint8_t)Graph[clique[i].name()][clique[j].name()] != '0'){ 
                clique[i].degree++;
                cout << "Incrementing: " << clique[i] << " At x,y " << i << " " << j << endl;
            }
        }
    }
}
void ReadGraph(string filename, uint8_t **graph, vector<Node>& nodeList){   // Read the graph from a file
    ifstream file;
    file.open(filename);
    if(file.is_open()){
        for(int i = 0; i < NODES; i++){
            file >> nodeList[i];
            for(int j = 0; j <= i; j++){
                if (i == j) graph[i][j] = '1';
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

void PrintGraph(uint8_t **graph, vector<Node>& nodeList){
    for(int i = 0; i < NODES; i++){
        cout << nodeList[i] << " ";
        for(int j = 0; j < NODES; j++){
            cout << graph[i][j] << " ";
        }
        cout << endl;
    }
}
bool VerifyClique(uint8_t **graph, vector<Node>& clique){
    for(int i = 0; i < (int)clique.size(); i++){
        for(int j = i+1; j < (int)clique.size(); j++){
            if(graph[clique[i].name()][clique[j].name()] == '0'){   // If there is no connection between the nodes we return false
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
    graph = new uint8_t*[NODES]; // Allocate memory for the graph
    for(int i = 0; i < NODES; i++){
        graph[i] = new uint8_t[NODES];
    }
    nodeList.resize(NODES);

    ReadGraph(filename, graph, nodeList);
    // find degree
    vector<Node> test = nodeList;

    // Timer
    auto start = chrono::high_resolution_clock::now();
    CountInvEdges(graph, nodeList);
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "Time: " << duration.count() << " microseconds" << endl;
////////////////////////////////////////////////////////////////
    // Find the largest vertex cover
    vector<Node> vertexCover;
    vector<Edge> edges;
    // populate edges with inverse graph
    for(int i = 0; i < NODES; i++){
        for(int j = 0; j < i; j++){
            if(graph[i][j] == '0'){ // 1 for regular, 0 for INVERSE
                Edge e(nodeList[i], nodeList[j]);
                edges.push_back(e);
            }
        }
    }
    // sort edges with largest first
    sort(edges.begin(), edges.end());

    // if debug print edges
    if(DEBUG){
        cout << "Edges: " << edges.size() << endl;
        for(int i = 0; i < (int)edges.size(); i++){
            cout << edges[i].uName() << "-" << edges[i].vName() << " " << edges[i].degree() << endl;
        }
        cout << endl;
    }
    // Find the vertex cover
    while(!edges.empty()){
        // Edge e = edges[0];
        Edge e = edges[edges.size() - 1];
        cout << "Adding: " << e << " " << e.degree() << endl;
        vertexCover.push_back(e.getU()); // Add the node to the vertex cover
        if(edges.size() > 1) vertexCover.push_back(e.getV()); // If there is more than one edge left, add the other node to the vertex cover
        // edges.erase(edges.begin());
        cout << "Removing: " << e << endl;
        edges.erase(edges.end() - 1);   // Remove the edge from the list
        // Remove all edges that contain the nodes
        for(int i = 0; i < (int)edges.size(); i++){
            if(edges[i].getU() == e.getU() || edges[i].getU() == e.getV() || edges[i].getV() == e.getU() || edges[i].getV() == e.getV()){
                edges.erase(edges.begin() + i);
                i--;
            }
        }
    }
    ////////////////////////////////////////////////////////////////////
    // Find the largest clique
    vector<Node> maxClique;
    if(DEBUG){
        cout << "Vertex Cover: ";
        for(int i = 0; i < (int)vertexCover.size(); i++){
            cout << vertexCover[i] << " ";
        }
        cout << endl;
    }
    vector<Node> independentSet; // Independent Set(G') = Graph - Vertex Cover(G')
                                 // Independent Set (G') = Max Clique(G)
                                 // Max Clique(G) = Graph - Vertex Cover(G')
    // Independent Set = Graph - Vertex Cover
    for(int i = 0; i < NODES; i++){
        if(find(vertexCover.begin(), vertexCover.end(), nodeList[i]) == vertexCover.end()){
            independentSet.push_back(nodeList[i]);
        }
    }
    // 
    cout << "Max Clique: ";
    for(int i = 0; i < (int)independentSet.size(); i++){
        cout << independentSet[i] << " ";
    }
    cout << endl << "Size: " << independentSet.size() << endl;
    cout << "Is clique "  << VerifyClique(graph, independentSet);
    cout << endl;
    ////////////////////////////////////////////////////////////////////
    

    
    
    
    return 0;
}
