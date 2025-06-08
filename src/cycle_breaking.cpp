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

// ==== CBSolver 實作 ====
CBSolver::CBSolver(bool isDirected, int nodeCount, int edgeCount)
    : directed(isDirected), n(nodeCount), m(edgeCount) {}

void CBSolver::addEdge(int u, int v, int w, set<pair<int, int>>& edge_seen) {
    if (w < 0) return;
    if (directed) {
        edges.push_back({u, v, w});
    } else {
        auto key = minmax(u, v);
        if (!edge_seen.count(key)) {
            edge_seen.insert(key);
            edges.push_back({u, v, w});
        }
    }
}

bool CBSolver::hasCycleDFS(int u, vector<vector<int>>& adj, vector<bool>& visited, vector<bool>& recStack) {
    visited[u] = recStack[u] = true;
    for (int v : adj[u]) {
        if (!visited[v] && hasCycleDFS(v, adj, visited, recStack)) return true;
        if (recStack[v]) return true;
    }
    recStack[u] = false;
    return false;
}

bool CBSolver::wouldFormCycle(int u, int v, int n, vector<vector<int>>& adj) {
    adj[u].push_back(v);
    vector<bool> visited(n, false), recStack(n, false);
    bool cycle = hasCycleDFS(u, adj, visited, recStack);
    adj[u].pop_back();
    return cycle;
}

void CBSolver::process(ofstream& fout) {
    sort(edges.begin(), edges.end());
    DSU dsu(n);
    vector<vector<int>> adj(n);
    vector<Edge> removed;
    int total_removed_weight = 0;

    for (const auto& e : edges) {
        if (directed) {
            if (dsu.unite(e.u, e.v)) {
                adj[e.u].push_back(e.v);
            } else {
                if (wouldFormCycle(e.u, e.v, n, adj)) {
                    removed.push_back(e);
                    total_removed_weight += e.w;
                } else {
                    adj[e.u].push_back(e.v);
                }
            }
        } else {
            if (!dsu.unite(e.u, e.v)) {
                removed.push_back(e);
                total_removed_weight += e.w;
            }
        }
    }

    if (removed.empty()) {
        fout << "0\n";
    } else {
        fout << total_removed_weight << '\n';
        for (const auto& e : removed) {
            fout << e.u << " " << e.v << " " << e.w << '\n';
        }
    }
}