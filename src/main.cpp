
// **************************************************************************
//  File       [main.cpp]
//  Author     [I-An Hou]
//  Synopsis   [The main program of 2025 spring Algorithm PA3]
// **************************************************************************


#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <set>
#include "../lib/tm_usage.h"
#include "cycle_breaking.h"
using namespace std;
#define ll long long

void helpMessage(int errorCode) {
    switch (errorCode) {
        case 0:
            cout << "Error: incorrect number of arguments.\n";
            break;
        default:
            cout << "Error: unknown error.\n";
    }
}
int main(int argc, char* argv[]) {
    if (argc != 3) {
        helpMessage(0);
        return EXIT_FAILURE;
    }

    CommonNs::TmUsage tmusg;
    CommonNs::TmStat stat;

    fstream fin(argv[1]);
    if (!fin) {
        helpMessage(1);
        return EXIT_FAILURE;
    }

    ofstream fout(argv[2]);
    if (!fout) {
        helpMessage(6);
        return EXIT_FAILURE;
    }

    while (true) {
        string type;
        fin >> type;
        if (type == "0" || fin.eof()) break;

        bool directed = (type == "d");
        int n, m;
        fin >> n >> m;
        if (!fin) {
            helpMessage(3);
            return EXIT_FAILURE;
        }

        CBSolver cb(directed, n, m);
        set<pair<int, int>> edge_seen;

        for (int i = 0; i < m; ++i) {
            int u, v, w;
            fin >> u >> v >> w;
            if (!fin) {
                helpMessage(4);
                return EXIT_FAILURE;
            }
            cb.addEdge(u, v, w, edge_seen);
        }

        cb.process(fout);
    }

    tmusg.getPeriodUsage(stat);
    cout << "The total CPU time: " << (stat.uTime + stat.sTime) / 1000.0 << "ms\n";
    cout << "Memory: " << stat.vmPeak << "KB\n";

    fin.close();
    fout.close();
    return EXIT_SUCCESS;
}