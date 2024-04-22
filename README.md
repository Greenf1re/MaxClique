# MaxClique
3 Approaches to the Max Clique problem. Also provides a graph generator 
- Bruteforce
- Vertex Cover (G')
- Original, statistic approach

 

# Getting Started
To compile each program: type "make" + "program name" in the command line. The main program of interest is Original.exe, compiled with ```make original```. The relevant source code is *OriginalMaxClique.cpp*
## Programs
- Original: my original approach to solving Max Clique. use ```make``` to compile with optimizations, or ```make original``` for regular compilation
- ```./Original.exe <graph file> <node count>```
- Generator: can be used to create graphs with a given size and density. Compile with ```make generator``` 
- ```./Generator <graph file> <node count> <density%>```
- Verifier: Used to verify a solution to a graph file. Compiled with ```make verifier```
- ```./Verifier.exe <graph file> <Solution> <count>```
- Density: finds the density and other statistics about the graph ```make density```
- ```./Density.exe <graph file> <node count>```
- Bruteforce: bruteforce the max clique out of a graph. Works in reasonable time for up to ~30 nodes. Compile with ```make bruteforce```
- ```./BruteForceMaxClique.exe <graph file> <node count>```
- Heuristic: Vertex Cover to Max Clique. Compile with ```make heuristic```
- ```./HeuristicMaxClique.exe <graph file> <node count>```
- To clean files, run ```make clean```

## Original Approach Description
When I started this project, the first thing I noticed was that we were only working with binary data. I initially came up with the idea of using a logical AND between rows to quickly find which nodes they have in common.
This evolved into finding the similarity between 2 rows A and B. I could pick the row with the most nodes (A) and find its most similar match (B), then AND them to C and run the process iteratively with A=C --> B=similar(C ).
However, I tweaked the similarity function to only rank positive matches, which produced much better results.
The basic steps in the final algorithm:
1. Generate a positive similarity matrix
2. Find 2 most similar rows (regardless of their edge counts)
3. Then, iteratively:
4. C = A AND B
5. B = row in Graph most similar to C
6. C AND B
7. Until we run out of nodes.

# Notes
I used O3 to run compiler optimizations which greatly improved my running times. Some of these statistics are found in NOTES.txt
# Files in the project:
- 6 cpp files for each program
- Makefile
- Notes
- Readme
- Solutions folder
