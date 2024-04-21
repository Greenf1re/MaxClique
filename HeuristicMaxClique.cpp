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
            return max1 < max2;
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
void CountInvEdges(uint8_t** Graph, vector<Node>& clique){
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
void CountEdges(uint8_t** Graph, vector<Node>& clique){
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
    // find degree
    vector<Node> test = nodeList;
    CountEdges(graph, test);
    cout << "TEST: " << endl;
    CountInvEdges(graph, nodeList);
////////////////////////////////////////////////////////////////
    vector<Node> vertexCover;
    vector<Edge> edges;
    // populate edges
    for(int i = 0; i < NODES; i++){
        for(int j = 0; j < i; j++){
            if(graph[i][j] == '0'){ // INVERSE
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
    while(!edges.empty()){
        // Edge e = edges[0];
        Edge e = edges[edges.size() - 1];
        cout << "Adding: " << e << " " << e.degree() << endl;
        vertexCover.push_back(e.getU());
        if(edges.size() > 1) vertexCover.push_back(e.getV());
        // edges.erase(edges.begin());
        cout << "Removing: " << e << endl;
        edges.erase(edges.end() - 1);
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
    vector<Node> independentSet;
    // Independent Set = Graph - Vertex Cover
    for(int i = 0; i < NODES; i++){
        if(find(vertexCover.begin(), vertexCover.end(), nodeList[i]) == vertexCover.end()){
            independentSet.push_back(nodeList[i]);
        }
    }
    // 
    if(DEBUG){
        cout << "Max Clique: ";
        for(int i = 0; i < (int)independentSet.size(); i++){
            cout << independentSet[i] << " ";
        }
        cout << "Is clique "  << VerifyClique(graph, independentSet);
        cout << endl;
    }
    ////////////////////////////////////////////////////////////////////
    

    
    
    
    return 0;
}
