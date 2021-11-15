/*
ECE-365 Project 3: dijkstra
Layth Yassin
Professor Sable

Program finds the shortest path to each node using Dijkstra's algorithm given a text
file specifying the graph and a starting vertex. Outputs results to user-specified file.
*/

#include "graph.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

// populates graph object with nodes from graph file
void populateGraph(const std::string graphFileName, graph &g) {
    std::ifstream graphInput;
    graphInput.open(graphFileName);
    std::string line;
    
    // tokenize input line-by-line and insert nodes into graph
    while (getline(graphInput, line)) {
        std::string nodeName1, nodeName2, weight;
        std::istringstream ss(line);
        ss >> nodeName1 >> nodeName2 >> weight;
        g.insert(nodeName1, nodeName2, weight);
    }

    graphInput.close();
}

double getCpuTime() {
    return (double) clock() / CLOCKS_PER_SEC;
}

int main() {
    graph g;
    std::ofstream output;
    std::string graphFileName, outputFileName, sourceVertex;
    
    // prompt user for graph file
    std::cout << "Enter name of graph file: ";
    std::cin >> graphFileName;

    // populate graph object with nodes from graph file
    populateGraph(graphFileName, g);

    // prompt user for source vertex until valid vertex given
    while (1) {
        std::cout << "Enter name of starting vertex: ";
        std::cin >> sourceVertex;
        if (g.isEncountered(sourceVertex)) {
            break;
        }
    }

    // perform dijkstra and output the time elapsed in seconds
    double start = getCpuTime();
    g.dijkstra(sourceVertex);
    double end = getCpuTime();
    std::cout << "Total time (in seconds) to apply Dijkstra's algorithm: " << end - start << std::endl;

    // prompt user for output file and populate file with paths from source to other nodes
    std::cout << "Enter name of output file: ";
    std::cin >> outputFileName;
    g.outputPaths(outputFileName);

    return 0;
}
