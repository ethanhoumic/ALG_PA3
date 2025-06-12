#include <iostream>
#include <fstream>
#include <set>
#include <random>
#include <chrono>

using namespace std;

set<pair<int, int>> undirectedEdgeSet;

void generateUndirected(const string& filename) {
    int n = 100000;
    long long m = 1000000;
    ofstream fout(filename);
    fout << "u\n";  // 無向圖

    fout << n << " " << m << "\n";
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> dist(0, n - 1);
    uniform_int_distribution<int> wdist(-1000, 1000);

    while (undirectedEdgeSet.size() < m) {
        int u = dist(rng);
        int v = dist(rng);
        if (u == v) continue;
        if (u > v) swap(u, v);
        auto p = make_pair(u, v);
        if (undirectedEdgeSet.insert(p).second) {
            if (undirectedEdgeSet.size() % 100000 == 0) {
                cout << "Generated " << undirectedEdgeSet.size() << " edges\n"; // Debug output
            }
            fout << u << " " << v << " " << wdist(rng) << "\n";
        }
    }
    fout << "0\n";
    fout.close();
}

void generateDirected(const string& filename) {
    int n = 50;
    int m = 100;
    ofstream fout(filename);
    fout << "d\n";  // 有向圖

    fout << n << " " << m << "\n";
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> dist(0, n - 1);
    uniform_int_distribution<int> wdist(-1000, 1000);
    set<pair<int, int>> seen;

    while (seen.size() < m) {
        int u = dist(rng);
        int v = dist(rng);
        if (u == v) continue;
        auto p = make_pair(u, v);
        if (seen.insert(p).second) {
            if (seen.size() % 1000 == 0) cout << "Adding edge: " << u << " " << v << "\n"; // Debug output
            fout << u << " " << v << " " << wdist(rng) << "\n";
        }
    }
    fout << "0\n";
    fout.close();
}

int main() {
    generateUndirected("inputs/undirected.in");
    //generateDirected("inputs/directed.in");
    return 0;
}
