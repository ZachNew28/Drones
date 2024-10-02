// Project Identifier: 1761414855B69983BD8035097EFBD312EB0527F0

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

struct Point {
    int x, y;
};

class FASTTSP {
    vector<pair<int, double>> path;   // Stores the index and edge cost to next index
    double findDistance(const size_t i, const size_t j);

public:
    void read();
    void run();
    void print();
    vector<pair<int, double>> getPath() { return path; }
    bool twoOptSwap(size_t i, size_t k);

    double totalDist = 0;
    vector<Point> vertices;
};
