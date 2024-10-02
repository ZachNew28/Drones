// Project Identifier: 1761414855B69983BD8035097EFBD312EB0527F0

#include "fasttsp.h"

// Calculates the Euclidean distance between two vertices in the graph
double FASTTSP::findDistance(const size_t i, const size_t j) {
    double dx = vertices[i].x - vertices[j].x;   // Difference in x-coordinates
    double dy = vertices[i].y - vertices[j].y;   // Difference in y-coordinates
    return sqrt((dx * dx) + (dy * dy));          // Return the Euclidean distance
}

// Reads vertex data from standard input
void FASTTSP::read() {
    size_t size;
    cin >> size;             // Read the number of vertices
    vertices.resize(size);   // Resize the vector to hold the vertices

    for (size_t i = 0; i < size; i++) {
        cin >> vertices[i].x >> vertices[i].y;   // Read each vertex's coordinates
    }
}

// Main algorithm for constructing a fast approximation of the TSP tour
void FASTTSP::run() {
    // Initially connect the first two vertices
    double initialDist = findDistance(0, 1);   // Calculate distance between the first two vertices
    path.push_back({ 0, initialDist });        // Add first vertex and distance to the path
    path.push_back({ 1, initialDist });        // Add second vertex and distance to the path

    // Insert each subsequent vertex at the best position
    for (size_t i = 2; i < vertices.size(); ++i) {
        double minDist = numeric_limits<double>::infinity();   // Start with an infinitely large minimum distance
        int minIndex = -1;                                     // Initialize the index where insertion is optimal

        // Evaluate insertion between every pair of consecutive vertices
        for (size_t j = 0; j < path.size() - 1; ++j) {
            double costRemove = path[j].second;   // Cost of the edge that will be removed
            double costAdd1 = findDistance(static_cast<size_t>(path[j].first),
                                           i);   // Cost of the new edge from path vertex to new vertex
            double costAdd2 = findDistance(
              i, static_cast<size_t>(path[j + 1].first));   // Cost of the new edge from new vertex to next path vertex
            double costTotal = costAdd1 + costAdd2 - costRemove;   // Total new cost if this insertion is performed

            if (costTotal < minDist) {
                minDist = costTotal;              // Update the minimum cost
                minIndex = static_cast<int>(j);   // Update the insertion index
            }
        }

        // Perform the insertion
        double newDistL = findDistance(static_cast<size_t>(path[static_cast<size_t>(minIndex)].first),
                                       i);   // Distance from path vertex to new vertex
        double newDistR = findDistance(
          i, static_cast<size_t>(
               path[static_cast<size_t>(minIndex) + 1].first));      // Distance from new vertex to next path vertex
        path[static_cast<size_t>(minIndex)].second = newDistL;       // Update the distance of the existing path edge
        path.insert(path.begin() + minIndex + 1, { i, newDistR });   // Insert the new vertex and distance in the path
    }

    // Get the total distance
    for (size_t i = 0; i < path.size(); ++i) {
        totalDist += path[i].second;
    }
}

// Prints the approximate TSP tour and its total distance
void FASTTSP::print() {
    cout << fixed << totalDist << "\n";   // Print the total distance
    for (const auto& p : path) {
        cout << p.first << " ";   // Print each vertex in the tour
    }
}
