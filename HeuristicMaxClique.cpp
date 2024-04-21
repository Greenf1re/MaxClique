#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#define DEBUG true
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
class Edge{
    int u;
    int v;
    public:
        Edge(int u, int v){
            this->u = u;
            this->v = v;
        }
        int uName(){
            return u;
        }
        int vName(){
            return v;
        }
};
void ReadGraph(string filename, uint8_t **graph, vector<Node>& nodeList){
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
            if(graph[clique[i].name()][clique[j].name()] == '0'){
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
    uint8_t **graphInv = NULL;
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
    graph = new uint8_t*[NODES];
    for(int i = 0; i < NODES; i++){
        graph[i] = new uint8_t[NODES];
    }
    nodeList.resize(NODES);

    ReadGraph(filename, graph, nodeList);
    // print the graph
    // if(DEBUG){
    //     PrintGraph(graph, nodeList);
    // }
    ////////////////////////////////////////////////////////////////////
    // O(E) vertex cover algorithm that guarentees no worse than 2x solution
    ////////////////////////////////////////////////////////////////////
    // Find a max clique using this Vertex Cover pseudocode:
    // "C = NULL
    // E’ = G.E
    // While E’ != NULL
    //     Let (u,v) be an arbitrary edge of E’
    //     C = C UNION {u,v}
    //     Remove from E’ edge (u,v) and every edge incident on either u or v
    // Return C
    // "
    // Populate inverse of graph
    graphInv = new uint8_t*[NODES];
    for(int i = 0; i < NODES; i++){
        graphInv[i] = new uint8_t[NODES];
    }
    
    for(int i = 0; i < NODES; i++){
        for(int j = 0; j <= i; j++){
            if (i == j) graphInv[i][j] = '0';
            if(graphInv[i][j] == '1'){
                graphInv[i][j] = '0';
                graphInv[j][i] = '0';
            }
            else{
                graphInv[i][j] = '1';
                graphInv[j][i] = '1';
            }
        }
    }
    vector<Node> vertexCover;
    vector<Edge> edges;
    // populate edges
    for(int i = 0; i < NODES; i++){
        for(int j = 0; j < i; j++){
            if(graph[i][j] == '0'){
                Edge e(i, j);
                edges.push_back(e);
            }
        }
    }
    // if debug print edges
    if(DEBUG){
        cout << "Edges: " << edges.size() << endl;
        for(int i = 0; i < (int)edges.size(); i++){
            cout << edges[i].uName() << "-" << edges[i].vName() << " ";
        }
        cout << endl;
    }
    while (!edges.empty()){
        Edge e = edges.back();
        edges.pop_back();
        vertexCover.push_back(Node(e.uName()));
        vertexCover.push_back(Node(e.vName()));
        for(int i = 0; i < (int)edges.size(); i++){
            if(edges[i].uName() == e.uName() || edges[i].vName() == e.uName() || edges[i].uName() == e.vName() || edges[i].vName() == e.vName()){
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
    vector<Node> independentSet;
    // Independent Set = Graph - Vertex Cover
    for(int i = 0; i < NODES; i++){
        if(find(vertexCover.begin(), vertexCover.end(), nodeList[i]) == vertexCover.end()){
            independentSet.push_back(nodeList[i]);
        }
    }
    // 
    if(DEBUG){
        cout << "Independent Set: ";
        for(int i = 0; i < (int)independentSet.size(); i++){
            cout << independentSet[i] << " ";
        }
        cout << endl;
    }
    ////////////////////////////////////////////////////////////////////
    

    
    
    
    return 0;
}
