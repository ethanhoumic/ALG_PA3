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

struct Node{
    int data, weight;
    Node* next;
};

struct Edge{
    int u, v, w;
};

// Disjoint Set
struct DS {
    int parent, rank;
};

class CBSolver {
public:
    CBSolver(int n, int m, vector <pair<pair<int, int>, int>> edges, bool directed);
    ~CBSolver();
    void solve(std::ofstream& fout);
private:
    bool directed;
    int n, m;
    //int update;
    vector <pair<pair<int, int>, int>> edges;
    Node** adjList;
    DS* ds;

    // Union-Find
    int findRoot(int i);
    void unionDS(int a, int b);

    // Graph functions
    void addAdjEdge(int u, int v, int w);
    void deleteEdge(int u);
    bool findCycle();
    bool DFS(int u, bool visited[], bool stack[]);

    // Kruskal algs
    void solveUndirected(std::ofstream& fout);
    void solveDirected(std::ofstream& fout);
};

#endif