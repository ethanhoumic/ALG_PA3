// **************************************************************************
//  File       [cycle_breaking.cpp]
//  Author     [I-An Hou]
//  Synopsis   [Supporting program of 2025 spring Algorithm PA3]
// **************************************************************************
#include "cycle_breaking.h"
#include <algorithm>
#include <iostream>
#include <utility>
using namespace std;

CBSolver::CBSolver(int n, int m, vector <pair<pair<int, int>, int>> edges, bool directed)
    : n(n), m(m), directed(directed), edges(edges){
    adjList = new Node*[n];
    for (int i = 0; i < n; ++i) {
        adjList[i] = nullptr;
    }
    ds = new DS[n];
    for (int i = 0; i < n; ++i) ds[i] = {i, 0};
}

CBSolver::~CBSolver() {
    for (int i = 0; i < n; ++i) {
        Node* curr = adjList[i];
        while (curr) {
            Node* temp = curr;
            curr = curr->next;
            delete temp;
        }
    }
    delete[] adjList;
    delete[] ds;
}

void CBSolver::solve(std::ofstream& fout){
    if (directed) solveDirected(fout);
    else solveUndirected(fout);
}

int CBSolver::findRoot(int i) {
    if (ds[i].parent != i) ds[i].parent = findRoot(ds[i].parent); // path compression
    return ds[i].parent;
}

void CBSolver::unionDS(int a, int b){
    int root_a = findRoot(a);
    int root_b = findRoot(b);

    // union by rank
    if (ds[root_a].rank > ds[root_b].rank) ds[root_b].parent = root_a;
    else if (ds[root_a].rank < ds[root_b].rank) ds [root_a].parent = root_b;
    else {
        ds[root_a].parent = root_b;
        ++ds[root_b].rank;
    }
}

void CBSolver::addAdjEdge(int u, int v, int w) {
    Node* node = new Node;
    node->data = v;
    node->weight = w;
    node->next = adjList[u];
    adjList[u] = node;
}

void CBSolver::deleteEdge(int u){
    if (adjList[u] == nullptr) return;
    Node* node = adjList[u];
    adjList[u] = adjList[u]->next;
    delete node;
}

bool CBSolver::DFS(int u, bool checked[], bool stack[]){
    checked[u] = true;
    stack[u] = true;
    for (Node* temp = adjList[u]; temp; temp = temp->next) {
        int v = temp->data;
        if (!checked[v] && DFS(v, checked, stack)) return true;
        else if (stack[v]) return true; // back edge → cycle
    }
    stack[u] = false;
    return false;
}

bool CBSolver::findCycle(){
    bool* checked = new bool[n]{};
    bool* stack = new bool[n]{};
    bool hasCycle = false;

    for (int i = 0; i < n; ++i) {
        if (!checked[i] && DFS(i, checked, stack)) {
            hasCycle = true;
            break;
        }
    }
    delete []checked;
    delete []stack;
    return hasCycle;
}

void CBSolver::solveUndirected(std::ofstream& fout){
    sort(edges.begin(), edges.end(), [](const pair<pair<int, int>, int>& lhs, const pair<pair<int, int>, int>& rhs) {
        return lhs.second > rhs.second;
    });
    long long int total = 0;
    vector <pair<pair<int, int>, int>> ev; // <<u, v>, w>

    for (int i = 0; i < m; ++i){
        int u = edges[i].first.first;
        int v = edges[i].first.second;
        int u_root = findRoot(u);
        int v_root = findRoot(v);
        if (u_root != v_root){
            unionDS(u_root, v_root);
        }
        else {
            ev.push_back({{u, v}, edges[i].second});
            total += edges[i].second;
        }
    }
    fout << total << endl;
    for (int i = 0; i < ev.size(); ++i){
        pair<pair<int, int>, int> e = ev[i];
        fout << e.first.first << " " << e.first.second << " " << e.second << endl;
    }
}

void CBSolver::solveDirected(std::ofstream& fout) {
    sort(edges.begin(), edges.end(), [](const pair<pair<int, int>, int>& lhs, const pair<pair<int, int>, int>& rhs) {
        return lhs.second > rhs.second;
    });

    long long int total = 0;
    vector <pair<pair<int, int>, int>> forDS; // <<u, v>, w>
    vector <pair<pair<int, int>, int>> remove; // <<u, v>, w>

    for (int i = 0; i < m; ++i) {
        int u = edges[i].first.first, v = edges[i].first.second, w = edges[i].second;
        if (findRoot(u) != findRoot(v)) {
            unionDS(u, v);
            addAdjEdge(u, v, w);
        }
        else {
            forDS.push_back({{u, v}, w});
            total += w;
        }
    }

    for (int i = 0; i < forDS.size(); ++i) {
        pair<pair<int, int>, int> e = forDS[i];
        if (e.second < 0) {
            // update++;
            remove.push_back(e);
            continue;
        }

        addAdjEdge(e.first.first, e.first.second, e.second);
        if (findCycle()) {
            //update++;
            deleteEdge(e.first.first);
            remove.push_back(e);
        }
        else total -= e.second;
    }

    fout << total << endl;
    for (int i = 0; i < remove.size(); ++i) {
        pair<pair<int, int>, int> e = remove[i];
        fout << e.first.first << " " << e.first.second << " " << e.second << endl;
    }
    // cout << "Total update: " << update << "\n"; // Debug output
}