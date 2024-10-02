// Project Identifier: 1761414855B69983BD8035097EFBD312EB0527F0

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

enum class LocationType { MainCampus, MedicalCampus, Border };

struct PointMST {
    int x, y;
    LocationType type;
};

class MST {
private:
    vector<PointMST> points;
    vector<double> minEdge;
    vector<int> prevVertex;
    vector<bool> visited;

    double calculateDistance(size_t i, size_t j);

public:
    void readPoints();
    void runMST();
};
