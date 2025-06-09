
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
        case 1:
            cout << "Error: input file format error.\n";
            break;
        case 2:
            cout << "Error: cannot open output file.\n";
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
        helpMessage(2);
        return EXIT_FAILURE;
    }

    while (true) {
        string type;
        fin >> type;
        if (type == "0" || fin.eof()) break;

        bool directed = (type == "d");
        int n, m;
        fin >> n >> m;

        Edge* edges = new Edge[m];
        for (int i = 0; i < m; ++i) {
            fin >> edges[i].u >> edges[i].v >> edges[i].w;
        }

        CBSolver cb(n, m, edges, directed);
        cb.solve(fout);

        delete []edges;
    }

    tmusg.getPeriodUsage(stat);
    cout << "The total CPU time: " << (stat.uTime + stat.sTime) / 1000.0 << "ms\n";
    cout << "Memory: " << stat.vmPeak << "KB\n";

    fin.close();
    fout.close();
    return EXIT_SUCCESS;
}