// **************************************************************************
//  File       [cycle_breaking.cpp]
//  Author     [I-An Hou]
//  Synopsis   [Supporting program of 2025 spring Algorithm PA3]
// **************************************************************************
#include "cycle_breaking.h"
#include <algorithm>
#include <iostream>
using namespace std;

DSU::DSU(int n) : parent(n), rank(n, 0) {
    for (int i = 0; i < n; ++i) parent[i] = i;
}

int DSU::find(int x) {
    return parent[x] == x ? x : parent[x] = find(parent[x]);
}

bool DSU::unite(int x, int y) {
    int xr = find(x), yr = find(y);
    if (xr == yr) return false;
    if (rank[xr] < rank[yr]) parent[xr] = yr;
    else {
        parent[yr] = xr;
        if (rank[xr] == rank[yr]) rank[xr]++;
    }
    return true;
}

CBSolver::CBSolver(bool isDirected, int nodeCount, int edgeCount)
    : directed(isDirected), n(nodeCount), m(edgeCount) {}

void CBSolver::addEdge(int u, int v, int w, set<pair<int, int>>& edge_seen) {
    if (directed) {
        edges.push_back({u, v, w});
    }
    else {
        auto key = minmax(u, v);
        if (!edge_seen.count(key)) {
            edge_seen.insert(key);
            edges.push_back({u, v, w});
        }
    }
}

// DFS cycle detection for directed graphs
// color: 0=white, 1=gray, 2=black
bool CBSolver::hasCycleDFS(int u, vector<vector<int>>& adj, vector<int>& color) {
    color[u] = 1; // gray
    for (int v : adj[u]) {
        if (color[v] == 1) return true; // back edge found
        if (color[v] == 0 && hasCycleDFS(v, adj, color)) return true; // cycle in neighbors
    }
    color[u] = 2; // black
    return false;
}

bool CBSolver::hasDirectedCycle(vector<vector<int>>& adj) {
    vector<int> color(n, 0);
    for (int i = 0; i < n; ++i) {
        if (color[i] == 0 && hasCycleDFS(i, adj, color)) {
            return true;
        }
    }
    return false;
}

void CBSolver::solveUndirected(ofstream& fout) {
    // Fast Maximum Spanning Forest approach
    sort(edges.begin(), edges.end());
    
    DSU dsu(n);
    int totalRemovedWeight = 0;
    vector<Edge> removedEdges;
    removedEdges.reserve(edges.size()); // Pre-allocate for efficiency
    
    // Process edges from highest weight to lowest (reverse order)
    for (int i = edges.size() - 1; i >= 0; i--) {
        int u = edges[i].u;
        int v = edges[i].v;
        
        if (dsu.unite(u, v)) {
            // Edge is part of MSF, keep it
        } else {
            // Edge creates cycle in undirected graph, remove it
            removedEdges.push_back(edges[i]);
            totalRemovedWeight += edges[i].w;
        }
    }
    
    // Output results
    if (removedEdges.empty()) {
        fout << "0\n";
    } else {
        fout << totalRemovedWeight << "\n";
        for (const auto& e : removedEdges) {
            fout << e.u << " " << e.v << " " << e.w << "\n";
        }
    }
}

void CBSolver::solveDirected(ofstream& fout) {
    
    // Use undirected MST to get initial candidate edges to remove
    sort(edges.begin(), edges.end());
    
    DSU dsu(n);
    vector<Edge> candidateRemove;
    vector<vector<int>> adjList(n);
    int totalWeight = 0;
    
    for (int i = m - 1; i >= 0; i--) {
        int u = edges[i].u;
        int v = edges[i].v;
        
        if (dsu.unite(u, v)) {
            // Add to directed graph
            adjList[u].push_back(v);
        }
        else {
            // Candidate for removal
            candidateRemove.push_back(edges[i]);
            totalWeight += edges[i].w;
        }
    }
    
    // Try to add back candidate edges in descending weight order
    // Sort candidates by weight in descending order
    sort(candidateRemove.rbegin(), candidateRemove.rend());
    
    vector<Edge> finalRemove;
    
    for (const auto& edge : candidateRemove) {
        if (edge.w < 0) {
            // Skip negative weight edges
            finalRemove.push_back(edge);
            continue;
        }
        
        int u = edge.u;
        int v = edge.v;
        
        // Try adding this edge
        adjList[u].push_back(v);
        
        if (hasDirectedCycle(adjList)) {
            // Creates cycle, remove it
            adjList[u].pop_back();
            finalRemove.push_back(edge);
        }
        else {
            // No cycle, can keep it
            totalWeight -= edge.w;
        }
    }
    
    int finalTotalWeight = 0;
    for (const auto& e : finalRemove) {
        finalTotalWeight += e.w;
    }
    
    if (finalRemove.empty()) fout << "0\n";
    else {
        fout << finalTotalWeight << "\n";
        for (const auto& e : finalRemove) {
            fout << e.u << " " << e.v << " " << e.w << "\n";
        }
    }
}

void CBSolver::process(ofstream& fout) {
    if (directed) {
        solveDirected(fout);
    } else {
        solveUndirected(fout);
    }
}