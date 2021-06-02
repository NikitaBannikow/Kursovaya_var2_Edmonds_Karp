// This program was made by Bannikov Nikita, group 9309
#include "Header.h"
#include <regex>
#include <iostream>
#include <fstream>

using namespace std;

class Util {
public:
    // Trims white-spaces and tabs
    static string trim(const string& str, const string& whitespace = " \t") {
        const auto strBegin = str.find_first_not_of(whitespace);
        if (strBegin == string::npos) {
            return ""; // no content
        }
        const auto strEnd = str.find_last_not_of(whitespace);
        const auto strRange = strEnd - strBegin + 1;

        return str.substr(strBegin, strRange);
    }

    // Split string by a delimeter
    static vector<string> split(const string& str, const string& delim) {
        vector<string> tokens;
        size_t prev = 0, pos = 0;
        do {
            pos = str.find(delim, prev);
            if (pos == string::npos) pos = str.length();
            string dirty_token = str.substr(prev, pos - prev);
            string token = trim(dirty_token);
            if (!token.empty()) tokens.push_back(token);
            prev = pos + delim.length();
        } while (pos < str.length() && prev < str.length());
        return tokens;
    }
};

// Adds an edge from node 'from' to node 'to' with certain capacity 
void Graph::addEdge(string from, string to, int capacity) {
    if (capacity < 0) {
        throw std::invalid_argument("Capacity must be positive");
    }

    string normalized_from = Util::trim(from);
    if (normalized_from.empty()) {
        throw std::invalid_argument("'from' shall not be empty");
    }

    string normalized_to = Util::trim(to);
    if (normalized_to.empty()) {
        throw std::invalid_argument("'to' shall not be empty");
    }

    int i_from = _nodePosition(normalized_from);
    int i_to = _nodePosition(normalized_to);

    //cout << "Adding from '" << from << "' (" << i_from << ") to '" << to << "'(" << i_to << ")" << endl;
    _addEdge(i_from, i_to, capacity, normalized_from, normalized_to);
}

// Calculates max. flow for the graph
long long Graph::maxFlow() {
    int _from = _getNodeIndex("S");

    if (_from == -1) {
        throw std::invalid_argument("S (Source) has not been set");
    }

    int _to = _getNodeIndex("T");

    if (_to == -1) {
        throw std::invalid_argument("T (Sink) has not been set");
    }

    return _maxFlow(_from, _to);
}

// Breadth first search method.
void Graph::_breadth_first_search(int v) {
    queue<int> q;
    used[v] = 1;
    q.push(v);
    while (!q.empty()) {
        v = q.front();
        q.pop();
        for (int e : g[v]) {
            int to = edges[e].other(v);
            if (!used[to] && edges[e].capacityTo(to)) {
                edgeTo[to] = e;
                used[to] = 1;
                q.push(to);
            }
        }
    }
}

// Calculates max. flow for the graph
long long Graph::_maxFlow(int from, int to) {
    long long flow = 0;
    if (debug_mode == true) cout << stateDump() << endl;
    while (_hasPath(from, to)) {
        int min_capacity = _minimalCapacityInPath(from, to);
        _addFlow(from, to, min_capacity);
        flow += min_capacity;
        if (debug_mode == true) cout << stateDump() << endl;
    }
    return flow;
}

// Defines if path exists
bool Graph::_hasPath(int from, int to) {
    fill(used.begin(), used.end(), 0);
    _breadth_first_search(from);
    return used[to];
}

// Adds an edge from node 'from' to node 'to' with certain capacity
void Graph::_addEdge(int from, int to, int capacity, string from_name, string to_name) {
    _resizeIfRequired();
    edges.push_back(Edge(from, to, capacity, from_name, to_name));
    g[from].push_back(edges.size() - 1);
    g[to].push_back(edges.size() - 1);
}
    
// Resize vectors if required
void Graph::_resizeIfRequired() {
    if (nodes.size() > g.size()) {
        g.resize(nodes.size());
        used.resize(nodes.size());
        edgeTo.resize(nodes.size());
    }
}

// Return an index of a node by name (if node is not found, -1 will be returned)
int Graph::_getNodeIndex(string node_name) {
    auto it = std::find(nodes.begin(), nodes.end(), node_name);
    if (it == nodes.end()) {
        return -1;
    }
    return std::distance(nodes.begin(), it);
}

// Adds (if required) a node by name, returning index int the nodes array
int Graph::_nodePosition(string node_name) {
    int index = _getNodeIndex(node_name);
    if (index == -1) {
        nodes.push_back(node_name);
        return nodes.size() - 1;
    }
    else {
        return index;
    }
}

// Defines minimal capacity on a path
int Graph::_minimalCapacityInPath(int from, int to) {
    int min_capacity = 1e9;
    // cout << "Start. from=" << nodes[from] << " to=" << nodes[to] << endl;
    for (int v = to; v != from; v = edges[edgeTo[v]].other(v)) {
        int capacity = edges[edgeTo[v]].capacityTo(v);
        // cout << edges[edgeTo[v]].from_name << "->" << edges[edgeTo[v]].to_name << "(" 
        //    << capacity << ") " << edges[edgeTo[v]].flow << endl;
        min_capacity = min(min_capacity, capacity);
    }
    return min_capacity;
}

// Returns state of graph.
// Format: from(flow/capacity)to sequence
string Graph::stateDump() {
    string buf;
    for (int i = 0; i < edges.size(); i++) {
        buf.append(edges[i].from_name);
        buf.append("(");
        buf.append(std::to_string(edges[i].flow));
        buf.append("/");
        buf.append(std::to_string(edges[i].capacity));
        buf.append(")");
        buf.append(edges[i].to_name);

        if (i < edges.size() - 1) {
            buf.append(" ");
        }
    }
    return buf;
}

// Adds flow from -> ... -> to
void Graph::_addFlow(int from, int to, int flow) {
    for (int v = to; v != from; v = edges[edgeTo[v]].other(v)) {
        edges[edgeTo[v]].addFlowTo(v, flow);
    }
}

// Loads graph from a file
//
// File format: 
//  1) a line is an edge: <from node name string> <to node name string> <capacity>
//  2) Example of of lines: 
//      S V 11
//      G5 G7 23
void Graph::load_from_file(const char* filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string str;
        int line_num = 0;
        while (std::getline(file, str)) {
            string delimiter = " ";
            vector<string> tokens = Util::split(str, delimiter);

            if (tokens.size() != 3) {
                throw std::invalid_argument("File invalid format");
            }
            else {
                addEdge(tokens.at(0), tokens.at(1), std::stoi(tokens.at(2)));
            }
        }
        file.close();
    } else {
        throw std::runtime_error("Can't open a file");
    }
}
