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

struct Edge {
    int u, v, w;
    bool operator<(const Edge& other) const {
        return w > other.w;
    }
};

class DSU {
    std::vector<int> parent, rank;
public:
    DSU(int n);
    int find(int x);
    bool unite(int x, int y);
};

class CBSolver {
    bool directed;
    int n, m;
    std::vector<Edge> edges;

    bool hasCycleDFS(int u, std::vector<std::vector<int>>& adj, std::vector<bool>& visited, std::vector<bool>& recStack);
    bool wouldFormCycle(int u, int v, int n, std::vector<std::vector<int>>& adj);

public:
    CBSolver(bool isDirected, int nodeCount, int edgeCount);
    void addEdge(int u, int v, int w, std::set<std::pair<int, int>>& edge_seen);
    void process(std::ofstream& fout);
};
#endif
