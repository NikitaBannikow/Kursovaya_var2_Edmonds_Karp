// This program was made by Bannikov Nikita, group 9309 
#pragma once
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <queue>
#include <iostream>
#include <locale>
#include <stdexcept>

using namespace std;

class Edge {
public:
    int from, to;
    string from_name;
    string to_name;
    int flow;
    int capacity;

    Edge(int from, int to, int capacity, string from_name, 
        string to_name) : from(from), to(to), capacity(capacity), from_name(from_name), to_name(to_name), flow(0) {}

    int other(int node) const {
        return node == from ? to : from;
    }

    int capacityTo(int node) const {
        return node == to ? capacity - flow : flow;
    }

    void addFlowTo(int node, int f) {
        flow += (node == to ? f : -f);
    }

};

class Graph {
    // Keeps nodes names
    vector<string> nodes;

    // Keeps edges
    vector<Edge> edges;

    // Keeps the graph
    vector<vector<int>> g;

    // Keeps current used nodes
    vector<bool> used;

    vector<int> edgeTo;

public:
    // Constructor
    Graph() {
    }

    // Adds an edge from node 'from' to node 'to' with certain capacity 
    void addEdge(string from, string to, int capacity);

    // Calculates max. flow for the graph
    long long maxFlow();

    // Loads graph from a file
    //
    // File format: 
    //  1) a line is an edge: <from node name string> <to node name string> <capacity>
    //  2) Example of of lines: 
    //      S V 11
    //      G5 G7 23
    void load_from_file(const char* filename);

    // Returns state of graph.
    // Format: from(flow/capacity)to sequence
    string stateDump();

    // Set debug mode (prints states of graph at each step)
    void setDebugMode(bool mode) {
        debug_mode = mode;
    }
private:
    bool debug_mode = false;

    // Breadth first search method.
    void _breadth_first_search(int v);

    // Defines minimal capacity on a path
    int _minimalCapacityInPath(int from, int to);

    // Calculates max. flow for the graph
    long long _maxFlow(int from, int to);

    // Adds an edge from node 'from' to node 'to' with certain capacity
    void _addEdge(int from, int to, int capacity, string from_name, string to_name);

    // Resize vectors if required
    void _resizeIfRequired();

    // Return an index of a node by name (if node is not found, -1 will be returned)
    int _getNodeIndex(string node_name);

    // Adds (if required) a node by name, returning index int the nodes array
    int _nodePosition(string node_name);

    // Adds flow from -> ... -> to
    void _addFlow(int from, int to, int flow);

    // Defines if path exists
    bool _hasPath(int from, int to);


};
