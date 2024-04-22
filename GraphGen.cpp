#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#define DEBUG false
using namespace std;
int NODES = 0;
// Generate a graph with a given density
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
                    file << (rand() % 100 < density ? 1 : 0) << " ";    // 1 if density is less than given 50, 0 otherwise
                }
            }
            file << endl;
        }
    }
}

int main(int argc, char** argv){
    vector<int> nodeList;   
    string filename;
    int density = 50;   // Default density is 50%
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
    
    DensityGraph(filename, density);    // Generate the graph
    
    return 0;
}