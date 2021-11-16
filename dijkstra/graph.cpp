/*
ECE-365 Project 3: dijkstra
Layth Yassin
Professor Sable

This file includes implementations of functions from graph class.
*/

#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <climits>
#include "graph.h"

#define hashSize 10000

graph::graph() {
    size = 0;
    hashTable mapping(hashSize);
}

// inserts node into graph
void graph::insert(const std::string &nodeName1, const std::string &nodeName2, const std::string &weight) {
    node *node1, *node2;

    // if node1 already encountered, locate node1 based off id
    if (isEncountered(nodeName1)) {
        node1 = static_cast<node *> (mapping.getPointer(nodeName1));
    }
    // if node1 has not been encountered, create a new node with this id
    else {
        node1 = new node;
        node1->id = nodeName1;
        node1->distance = INT_MAX;
        node1->known = false;
        vertices.push_back(node1);
        mapping.insert(nodeName1, node1);
        size++;
    }

    // if node2 already encountered, locate node2 based off id
    if (isEncountered(nodeName2)) {
        node2 = static_cast<node *> (mapping.getPointer(nodeName2));
    }
    // if node2 has not been encountered, create a new node with this id
    else {
        node2 = new node;
        node2->id = nodeName2;
        node2->distance = INT_MAX;
        node2->known = false;
        vertices.push_back(node2);
        mapping.insert(nodeName2, node2);
        size++;
    }

    // create edge and add to node1's adjacency list
    edge e;
    e.cost = stoi(weight);
    e.destinationVertex = node2;
    node1->adjList.push_back(e);
}

// determines if given node has already been encountered
bool graph::isEncountered(const std::string &id) {
    return mapping.contains(id);
}

// performs Dijkstra's algorithm given starting vertex to find shortest path to each node in graph
void graph::dijkstra(const std::string &id) {
    heap h(size);
    node *source = static_cast<node *> (mapping.getPointer(id));
    
    // initialize source vertex and insert it into the heap
    source->distance = 0;
    source->prev = nullptr;
    h.insert(id, source->distance, mapping.getPointer(id));

    std::string key;
    int val;
    void *tmp;
    while (!(h.deleteMin(&key, &val, &tmp))) {
        node *v = static_cast<node *> (tmp);
        v->known = true;
        // loop through all edges from v and update distances and prev nodes; insert destination vertices into heap
        for (std::list<edge>::const_iterator it = v->adjList.begin(); it != v->adjList.end(); it++) {
            if (v->distance + it->cost < it->destinationVertex->distance) {
                it->destinationVertex->distance = v->distance + it->cost;
                it->destinationVertex->prev = v;
                h.insert(it->destinationVertex->id, it->destinationVertex->distance, mapping.getPointer(it->destinationVertex->id));
            }
        }
    }
}

// outputs shortest paths from starting vertex to other nodes to output file
void graph::outputPaths(const std::string &outFile) {
    std::ofstream out;
    out.open(outFile);
    // loops through all nodes in the order they were encountered in the input file
    for (std::list<node *>::const_iterator it = vertices.begin(); it != vertices.end(); it++) {
        out << (*it)->id << ": ";
        // if distance of node is still INT_MAX, then there is no path to that node
        if ((*it)->distance == INT_MAX) {
            out << "NO PATH" << '\n';
        }
        // otherwise there is a path and it is retrieved by traversing pointers from the prev nodes until prev is the prev of the source
        else {
            out << (*it)->distance << " [";
            std::string path;
            node *tmp = (*it)->prev;
            while (tmp != nullptr) {
                path.insert(0, tmp->id + ", ");
                tmp = tmp->prev;
            }
            out << path << (*it)->id << "]" << '\n';
        }
    }
    out.close();
}
