// Project Identifier: 1761414855B69983BD8035097EFBD312EB0527F0

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>

#include "fasttsp.h"

using namespace std;

class OPTTSP : public FASTTSP {   // Inheriting from FASTTSP if using its methods and properties directly
public:
    OPTTSP() = default;
    void read();
    void run();
    void print();
    double findDistance(const size_t i, const size_t j);
    bool promising(size_t permLength, double runningTotal);
    void genPerms(size_t permLength, double runningTotal);
    double calculateMSTCostForUnvisited(size_t permLength);

private:
    std::vector<std::vector<double>> distanceMat;   // Matrix to hold distances between all pairs
    std::vector<double> minEdges;                   // Used in MST calculations
    std::vector<int> pathOpt;                       // Current permutation of the path
    std::vector<bool> visited;                      // Visited nodes tracking for MST
    double bestTotal;                               // Best path cost found
    std::vector<int> bestPath;                      // Best path sequence found
    std::vector<std::pair<int, double>> path;
};
