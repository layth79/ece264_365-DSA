/*
ECE-365 Project 3: dijkstra
Layth Yassin
Professor Sable

This is the header file that defines the graph class
*/

#ifndef _GRAPH_H
#define _GRAPH_H

#include "heap.h"
#include <list>
#include <string>

class graph {
    public:
        // graph constructor
        graph();

        // inserts node into graph
        void insert(const std::string &nodeName1, const std::string &nodeName2, const std::string &weight);

        // determines whether vertex has been previously encountered
        bool isEncountered(const std::string &id);

        // performs Dijkstra's algorithm given starting vertex to find shortest path to each node in graph
        void dijkstra(const std::string &id);

        // outputs shortest paths from starting vertex to other nodes to output file
        void outputPaths(const std::string &outFile);

    private:
        class node;
        class edge {
            public:
                int cost;
                node *destinationVertex;
        };

        class node {
            public:
                std::string id;
                int distance;
                bool known;
                std::list<edge> adjList; // adjacency list storing edges
                node *prev; // prev node to keep track of paths
        };

        int size; // number of nodes in graph
        std::list<node *> vertices; // linked list of vertices in graph
        hashTable mapping;
};

#endif //_GRAPH_H