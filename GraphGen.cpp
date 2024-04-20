#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#define DEBUG false
using namespace std;
int NODES = 0;

void DensityGraph(string filename, int density){
    ofstream file;
    file.open(filename);
    if(file.is_open()){
        for(int i = 0; i < NODES; i++){
            file << i << " ";
            for(int j = 0; j < i; j++){
                if(i == j){
                    file << 0 << " ";
                }
                else{
                    file << (rand() % 100 < density ? 1 : 0) << " ";
                }
            }
            file << endl;
        }
    }
}
void GenerateGraph(string filename){
    ofstream file;
    file.open(filename);
    if(file.is_open()){
        for(int i = 0; i < NODES; i++){
            file << i << " ";
            for(int j = 0; j < NODES; j++){
                if(i == j){
                    file << 0 << " ";
                }
                else{
                    file << rand() % 2 << " ";
                }
            }
            file << endl;
        }
    }
}
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
    vector<int> nodeList;
    string filename;
    int density = 50;
    if(argc < 4){
        cout << "CLI Usage: " << argv[0] << " <graph file> <count> <density%>" << endl;
        // return 1;
        cout << "Enter the number of nodes: ";
        cin >> NODES;
        cout << "Enter the filename: ";
        cin >> filename;
        cout << "Enter the density: ";
        cin >> density;
    }
    else{
        NODES = atoi(argv[2]);
        filename = argv[1];
        density = atoi(argv[3]);
    }
    
    DensityGraph(filename, density);
    
    return 0;
}