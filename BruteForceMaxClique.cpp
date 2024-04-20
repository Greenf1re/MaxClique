#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#define DEBUG false
using namespace std;
int NODES = 0;

void ReadGraph(string filename, int **graph, vector<int>& nodeList){
    ifstream file;
    file.open(filename);
    if(file.is_open()){
        for(int i = 0; i < NODES; i++){
            file >> nodeList[i];
            for(int j = 0; j < i; j++){
                file >> graph[i][j];
                graph[j][i] = graph[i][j];
            }
        }
    }
}
void PrintGraph(int **graph, vector<int>& nodeList){
    for(int i = 0; i < NODES; i++){
        cout << nodeList[i] << " ";
        for(int j = 0; j < NODES; j++){
            cout << graph[i][j] << " ";
        }
        cout << endl;
    }
}
bool VerifyClique(int **graph, vector<int>& clique){
    for(int i = 0; i < (int)clique.size(); i++){
        for(int j = i+1; j < (int)clique.size(); j++){
            if(graph[clique[i]][clique[j]] == 0){
                return false;
            }
        }
    }
    return true;
}
int main(int argc, char** argv){
    int **graph = NULL;
    vector<int> nodeList;
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
    graph = new int*[NODES];
    for(int i = 0; i < NODES; i++){
        graph[i] = new int[NODES];
    }
    nodeList.resize(NODES);

    ReadGraph(filename, graph, nodeList);
    // print the graph
    if(DEBUG){
        PrintGraph(graph, nodeList);
    }
    vector<int> maxClique;
    // Bruteforce using combinations of nodes
    // CODE_HERE
    int maxSize = 0;
    for (int size = 1; size <= NODES; size++) {
        vector<bool> v(NODES);
        fill(v.end() - size, v.end(), true);  // Initialize the combination vector
        do {
            vector<int> currentClique;
            for (int i = 0; i < NODES; i++) {
                if (v[i]) {
                    currentClique.push_back(i);
                }
            }
            if (VerifyClique(graph, currentClique)) {
                if ((int)currentClique.size() > maxSize) {
                    maxSize = currentClique.size();
                    maxClique = currentClique;
                    cout << "New Size: " << maxSize << endl;
                }
            }
        } while (next_permutation(v.begin(), v.end()));
    }
    ////////////////////////////////////////////////////////////////////
    cout << "Max Clique: ";
    for(int i = 0; i < (int)maxClique.size(); i++){
        cout << nodeList[maxClique[i]] << " ";
    }
    cout << endl;
    cout << "Size: " << maxClique.size() << endl;
    return 0;
}