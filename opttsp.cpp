// Project Identifier: 1761414855B69983BD8035097EFBD312EB0527F0

#include "opttsp.h"

#include <algorithm>
#include <cmath>
#include <exception>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

// Calculate Euclidean distance between two vertices
double OPTTSP::findDistance(const size_t i, const size_t j) {
    double dx = vertices[i].x - vertices[j].x;
    double dy = vertices[i].y - vertices[j].y;
    return sqrt((dx * dx) + (dy * dy));
}

// Evaluate whether the current permutation branch can lead to an optimal solution
bool OPTTSP::promising(size_t permLength, double runningTotal) {
    std::fill(minEdges.begin(), minEdges.end(), std::numeric_limits<double>::infinity());
    std::fill(visited.begin(), visited.end(), false);

    // Calculate the MST cost of the unvisited nodes for a better lower bound
    double mstCost = calculateMSTCostForUnvisited(permLength);

    double edgeTotal = 0;
    for (size_t i = permLength; i < pathOpt.size(); ++i) {
        if (!visited[i]) {
            double minEdgeToCurrent = std::numeric_limits<double>::infinity();
            for (size_t j = 0; j < permLength; ++j) {   // Check from all previously fixed nodes
                double currentDistance = distanceMat[static_cast<size_t>(pathOpt[j])][static_cast<size_t>(pathOpt[i])];
                if (currentDistance < minEdgeToCurrent) {
                    minEdgeToCurrent = currentDistance;
                }
            }
            edgeTotal += minEdgeToCurrent;
            if (edgeTotal + mstCost + runningTotal >= bestTotal) {
                return false;   // Not promising if the total cost exceeds the best known
            }
            visited[i] = true;
        }
    }

    return (runningTotal + edgeTotal + mstCost) < bestTotal;
}

// Generate all permutations of the remaining cities and evaluate them
void OPTTSP::genPerms(size_t permLength, double runningTotal) {
    if (permLength == pathOpt.size()) {
        double cycleCloseCost = distanceMat[static_cast<size_t>(pathOpt.back())][static_cast<size_t>(pathOpt[0])];
        if (runningTotal + cycleCloseCost < bestTotal) {
            bestTotal = runningTotal + cycleCloseCost;
            bestPath = pathOpt;   // Assuming bestPath is defined
        }
        return;
    }
    if (!promising(permLength, runningTotal)) {
        return;
    }
    for (size_t i = permLength; i < pathOpt.size(); ++i) {
        std::swap(pathOpt[permLength], pathOpt[i]);
        genPerms(
          permLength + 1,
          runningTotal
            + distanceMat[static_cast<size_t>(pathOpt[permLength - 1])][static_cast<size_t>(pathOpt[permLength])]);
        std::swap(pathOpt[permLength], pathOpt[i]);
    }
}

// Read vertices input from standard input
void OPTTSP::read() {
    FASTTSP::read();

    // Initialize distance matrix and other structures for MST and TSP calculations
    distanceMat.resize(vertices.size());
    minEdges.resize(vertices.size(), std::numeric_limits<double>::infinity());
    path.resize(vertices.size());
    pathOpt.resize(vertices.size());
    visited.resize(vertices.size());

    for (size_t i = 0; i < vertices.size(); ++i) {
        pathOpt[i] = static_cast<int>(i);
        distanceMat[i].resize(vertices.size());
        for (size_t j = 0; j < vertices.size(); ++j) {
            distanceMat[i][j] = findDistance(i, j);
        }
    }
}

// Print the best TSP path found and its total distance
void OPTTSP::print() {
    std::cout << bestTotal << "\n";
    for (auto i : bestPath) {
        std::cout << i << " ";
    }
    std::cout << "\n";
}

double OPTTSP::calculateMSTCostForUnvisited(size_t permLength) {
    size_t numVertices = pathOpt.size() - permLength;
    if (numVertices <= 1) {
        return 0;
    }

    std::vector<bool> inMST(numVertices, false);
    std::vector<double> minEdge(numVertices, std::numeric_limits<double>::infinity());
    minEdge[0] = 0;
    double totalMSTCost = 0;

    for (size_t i = 0; i < numVertices; ++i) {
        int nextVertex = -1;
        double smallestEdge = std::numeric_limits<double>::infinity();

        for (size_t j = 0; j < numVertices; ++j) {
            if (!inMST[j] && minEdge[j] < smallestEdge) {
                smallestEdge = minEdge[j];
                nextVertex = static_cast<int>(j);
            }
        }

        if (nextVertex == -1) {
            return std::numeric_limits<double>::infinity();   // Disconnected graph
        }

        inMST[static_cast<size_t>(nextVertex)] = true;
        totalMSTCost += smallestEdge;

        for (size_t j = 0; j < numVertices; ++j) {
            if (!inMST[j]) {
                double newWeight
                  = findDistance(static_cast<size_t>(pathOpt[permLength + static_cast<size_t>(nextVertex)]),
                                 static_cast<size_t>(pathOpt[permLength + j]));
                if (newWeight < minEdge[j]) {
                    minEdge[j] = newWeight;
                }
            }
        }
    }

    return totalMSTCost;
}

void OPTTSP::run() {
    // Create an instance of FASTTSP and configure it.
    FASTTSP fasttsp;
    fasttsp.vertices = this->vertices;   // Assuming vertices are set up and are needed by FASTTSP.

    // Run FASTTSP to find a fast solution.
    fasttsp.run();

    // Access FASTTSP results
    bestTotal = fasttsp.totalDist;   // Get the total distance calculated by FASTTSP.
    bestPath.clear();                // Clear any existing best path.

    for (const auto& p : fasttsp.getPath()) {
        bestPath.push_back(p.first);   // Collect vertex indices for the best path.
    }

    // Now, start the permutation generation with the initial setup.
    genPerms(1, 0);
}
