#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <mutex>
#include <thread>
#define DEBUG false
#define THREADED false
using namespace std;
std::mutex mlock; 

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
void ReadGraph(string filename, uint8_t **graph, vector<Node>& nodeList){   // Read the graph from the file
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
// Count the number of edges in the graph and store in clique vector, counting each node's degree
void CountEdges(uint8_t** Graph, vector<Node>& clique){ 
    for(int i = 0; i < (int)clique.size(); i++){
        for(int j = 0; j < (int)clique.size(); j++){
            if((uint8_t)Graph[clique[i].name()][clique[j].name()] != '0'){ 
                clique[i].degree++;
            }
        }
    }
}
// Calculate the average degree of the graph
int AverageDegree(vector<Node>& nodeList){
    double sum = 0;
    for(int i = 0; i < NODES; i++){
        sum += nodeList[i].degree;
    }
    return round(sum/(double)NODES);
}
// Print the graph
void PrintGraph(uint8_t **graph, vector<Node>& nodeList){
    for(int i = 0; i < NODES; i++){
        cout << nodeList[i] << " ";
        for(int j = 0; j < NODES; j++){
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
        if(row1[i] == row2[i]){
            count++;
        }
    }
    return count;
}
bool VerifyClique(uint8_t **graph, vector<Node>& clique){   // Verify if the clique is valid
    for(int i = 0; i < (int)clique.size(); i++){    // Check if all nodes in the clique are connected
        for(int j = i+1; j < (int)clique.size(); j++){  // Check if all nodes are connected to each other
            if(graph[clique[i].name()][clique[j].name()] == '0'){   // If there is no edge between the nodes
                return false;
            }
        }
    }
    return true;
}
void SaveCliqueToFile(vector<Node>& clique){    // Save the clique to a file
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

    if(argc < 3){ // If no arguments are passed, use cin
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
    nodeList.resize(NODES);  // Resize the nodeList to the number of nodes
    cout << "Reading Graph\n";
    ReadGraph(filename, graph, nodeList); // Read the graph from the file
    // print the graph
    if(DEBUG){
        PrintGraph(graph, nodeList); 
    }
    vector<Node> maxClique;
    ////////////////////////////////////////////////////////////////////
    // UNUSED CODE - SKIP
    // CountEdges(graph, nodeList);
    // for (int i = 0; i < NODES; i++){
    //     cout << nodeList[i] << " " << nodeList[i].degree << endl;
    // }
    // sort(nodeList.begin(), nodeList.end());
    // int avgDegree = AverageDegree(nodeList);
    // cout << "Average Degree: " << avgDegree << endl;
    //////////////////////////////////////////////////////////////////
    // Find node with average degree or closest
    // int index = 0;
    // for(int i = 0; i < NODES; i++){
    //     if(nodeList[i].degree >= avgDegree){
    //         index = i;
    //         break;
    //     }
    // }
    ////////////////////////////////////////////////////////////////////

    // Calculate similarity matrix
    cout << "Calculating Similarity Matrix\n";
    //start timer
    auto start = chrono::high_resolution_clock::now();
    int **similarity = new int*[NODES]; // Allocate memory for the similarity matrix
    for(int i = 0; i < NODES; i++){
        similarity[i] = new int[NODES];
    }
    int maxSimilarity = 0;  // Maximum similarity found
    int row1 = -1, row2 = -1;   // Indices with the maximum similarity
    if (!THREADED){ // If not threaded, calculate similarity matrix sequentially
        for(int i = 0; i < NODES; i++){
            similarity[i][i] = 0;
            for(int j = i+1; j < NODES; j++){
                // cout << "Row: " << i << " Compare: " << j << endl;
                similarity[i][j] = FindPosSimilarity(graph, graph[i], j);   // Calculate similarity. FindPosSimilarity only counts 1s that are in the same position
                similarity[j][i] = similarity[i][j];
                if(similarity[i][j] > maxSimilarity){
                    maxSimilarity = similarity[i][j];
                    row1 = i;
                    row2 = j;
                }
            }
        }
    }
    // Calculate similarity matrix with multitheading TODO
    
    //end timer
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Time: " << duration.count() << "ms" << endl;
    ////////////////////////////////////////////////////////////////////
    // Print similarity matrix
    if(DEBUG){
        for(int i = 0; i < NODES; i++){
            for(int j = 0; j < NODES; j++){
                cout << similarity[i][j] << " ";
            }
            cout << endl;
        }
    }

    cout << "Max Similarity: " << maxSimilarity << " Row1: " << row1 << " Row2: " << row2 << endl;
    vector<Node> triedCliques;  
    triedCliques.push_back(nodeList[row1]); 
    // triedCliques.push_back(nodeList[row2]); // we want to retry with node2 eventually
    vector<Node> clique;
    uint8_t *newRow = new uint8_t[NODES];   // New row to add to the clique
    for(int i = 0; i < NODES; i++){
        newRow[i] = graph[row1][i]; // Copy the first row to the new row
    }
    clique.push_back(nodeList[row1]);   // Add the first row to the clique
    clique.push_back(nodeList[row2]);   // Add the second row to the clique
    while(1){
        // cout << "Row1: " << row1 << " Row2: " << row2 << endl;
        for(int i = 0; i < NODES; i++){
            newRow[i] = newRow[i] == graph[row2][i] && graph[row2][i] != '0' ? '1' : '0';   // Logical AND of the two rows
        }
        // Find the row with the highest similarity to the new row
        int maxSim = -1;
        int maxRow = -1;
        for(int i = 0; i < NODES; i++){
            // Check if the row is already in the clique
            if(find(clique.begin(), clique.end(), nodeList[i]) != clique.end()) continue;
            // if(find(triedCliques.begin(), triedCliques.end(), nodeList[i]) != triedCliques.end()) continue;
            int sim = FindPosSimilarity(graph, newRow, i);
            if(sim > maxSim){
                maxSim = sim;
                maxRow = i;
            }
        }

        if(maxRow < 0){ // If no row was found, the clique is invalid
            // Find highest similarity in matrix that isn't in tried cliques
            for(int i = 0; i < NODES; i++){
                if(find(triedCliques.begin(), triedCliques.end(), nodeList[i]) != triedCliques.end()) continue;
                for(int j = 0; j < i; j++){
                    if(find(triedCliques.begin(), triedCliques.end(), nodeList[j]) != triedCliques.end()) continue;
                    if(similarity[i][j] > maxSim){
                        maxSim = similarity[i][j];
                        maxRow = j;
                        row1 = i;
                    }
                }
            }
            
            cout << "Invalid Clique " << (((double)maxSim / (double)NODES) * 100) << "%"<< endl << endl;

            // Copy the row to the new row
            for(int i = 0; i < NODES; i++){
                newRow[i] = graph[row1][i];
            }
            // Clear clique
            clique.clear();
            // Add the new rows to the clique
            clique.push_back(nodeList[row1]);
            triedCliques.push_back(nodeList[row1]);
            clique.push_back(nodeList[maxRow]);
            row2 = maxRow;
            // triedCliques.push_back(nodeList[maxRow]);
            // cout << "ROW1: " << row1 << " Row2: " << row2 << endl;
            if((int)triedCliques.size() == NODES){
                cout << "All nodes tried\n";
                break;
            }
            continue;

        }
        // Add the new row to the clique
        clique.push_back(nodeList[maxRow]);
        // cout << "VERIFYING CLIQUE" << maxRow << endl;
        // // print clique
        // for(int i = 0; i < (int)clique.size(); i++){
        //     cout << nodeList[clique[i].name()] << " ";
        // } cout << endl;
        //

        if(VerifyClique(graph, clique)){    // If the clique is valid
            if(clique.size() > maxClique.size()){
                maxClique = clique; // Set the max clique to the current clique
                cout << "MAX Clique: " << maxClique.size() << endl;
                for(int i = 0; i < (int)maxClique.size(); i++){
                    cout << nodeList[maxClique[i].name()] << " ";   // Print the max clique
                } cout << endl << endl << endl;
                SaveCliqueToFile(maxClique);
                //break;
            }
        }
        if((int)triedCliques.size() == NODES){
            cout << "All nodes tried\n";
            break;
        }
        // Find the next row to add
        row1 = row2;
        row2 = maxRow;
        
    }

    
    ////////////////////////////////////////////////////////////////////
    cout << "Max Clique: ";
    for(int i = 0; i < (int)maxClique.size(); i++){
        cout << nodeList[maxClique[i].name()] << " ";
    }
    cout << endl;
    cout << "Size: " << maxClique.size() << endl;
    return 0;
}