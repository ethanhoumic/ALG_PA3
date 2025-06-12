// **************************************************************************
//  File       [cycle_breaking.cpp]
//  Author     [I-An Hou]
//  Synopsis   [Supporting program of 2025 spring Algorithm PA3]
// **************************************************************************
#include "cycle_breaking.h"
#include <algorithm>
#include <iostream>
using namespace std;

EdgeVec::EdgeVec(int c): size(0), capacity(c) {
    data = new Edge[capacity];
}
EdgeVec::~EdgeVec() {
    delete []data;
}
void EdgeVec::pushBack(Edge e) {
    if (size == capacity) resize();
    data[size] = e;
    ++size;
}
Edge* EdgeVec::getEdge() const{
    return data;
}
int EdgeVec::getSize() const{
    return size;
}
void EdgeVec::resize(){
    capacity *= 2;
    Edge* newEdge = new Edge[capacity];
    for (int i = 0; i < size; ++i) newEdge[i] = data[i];
    delete []data;
    data = newEdge;
}

CBSolver::CBSolver(int n, int m, Edge* edges, bool directed)
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
    sort(edges, edges + m, [](const Edge& lhs, const Edge& rhs) {
        return lhs.w < rhs.w;
    });
    long long int total = 0;
    EdgeVec ev(n);

    for (int i = m - 1; i >= 0; --i){
        int u = edges[i].u;
        int v = edges[i].v;
        int u_root = findRoot(u);
        int v_root = findRoot(v);
        if (u_root != v_root){
            unionDS(u_root, v_root);
        }
        else {
            ev.pushBack(edges[i]);
            total += edges[i].w;
        }
    }
    fout << total << "\n";
    for (int i = 0; i < ev.getSize(); ++i){
        fout << ev.getEdge()[i].u << " " << ev.getEdge()[i].v << " " << ev.getEdge()[i].w << "\n";
    }
}

void CBSolver::solveDirected(std::ofstream& fout) {
    sort(edges, edges + m, [](const Edge& lhs, const Edge& rhs) {
        return lhs.w < rhs.w;
    });

    long long int total = 0;
    EdgeVec forDS(n);
    EdgeVec remove(n);

    for (int i = m - 1; i >= 0; --i) {
        int u = edges[i].u, v = edges[i].v;
        if (findRoot(u) != findRoot(v)) {
            unionDS(u, v);
            addAdjEdge(u, v, edges[i].w);
        }
        else {
            forDS.pushBack(edges[i]);
            total += edges[i].w;
        }
    }

    for (int i = 0; i < forDS.getSize(); ++i) {
        Edge e = forDS.getEdge()[i];
        if (e.w < 0) {
            // update++;
            remove.pushBack(e);
            continue;
        }

        addAdjEdge(e.u, e.v, e.w);
        if (findCycle()) {
            //update++;
            deleteEdge(e.u);
            remove.pushBack(e);
        }
        else total -= e.w;
    }

    fout << total << endl;
    for (int i = 0; i < remove.getSize(); ++i) {
        Edge e = remove.getEdge()[i];
        fout << e.u << " " << e.v << " " << e.w << endl;
    }
    // cout << "Total update: " << update << "\n"; // Debug output
}