// **************************************************************************
//  File       [cycle_breaking.h]
//  Author     [I-An Hou]
//  Synopsis   [Header of 2025 spring Algorithm PA3]
// **************************************************************************
#ifndef CYCLE_BREAKING_H
#define CYCLE_BREAKING_H

#include <vector>
#include <set>
#include <fstream>
using namespace std;

struct Edge {
    int u, v, w;
    bool operator<(const Edge& other) const {
        return w < other.w;
    }
};

// Disjoint Set
class DSU {
    std::vector<int> parent, rank;
public:
    DSU(int n);
    int find(int x);
    bool unite(int x, int y);
};

class CBSolver {
private:
    bool directed;
    int n, m;
    vector<Edge> edges;
    
    // For directed graph cycle detection
    bool hasCycleDFS(int u, vector<vector<int>>& adj, vector<int>& color);
    bool hasDirectedCycle(vector<vector<int>>& adj);
    
    // Directed case
    void solveDirected(ofstream& fout);
    
    // Undirected case
    void solveUndirected(ofstream& fout);

public:
    CBSolver(bool isDirected, int nodeCount, int edgeCount);
    void addEdge(int u, int v, int w, set<pair<int, int>>& edge_seen);
    void process(ofstream& fout);
};

#endif
