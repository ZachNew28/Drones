// Project Identifier: 1761414855B69983BD8035097EFBD312EB0527F0
#include "mst.h"

// Function that calculates the distance between to points
double MST::calculateDistance(size_t i, size_t j) {
    // Retrieve the points from the vectors using the indices provided.
    const auto& p1 = points[i];
    const auto& p2 = points[j];

    // Check if the points are on different campuses which are not directly connectable.
    if (((p1.type == LocationType::MainCampus && p2.type == LocationType::MedicalCampus)
         || (p1.type == LocationType::MedicalCampus && p2.type == LocationType::MainCampus))
        && (p1.type != LocationType::Border && p2.type != LocationType::Border)) {
        // Return infinity to indicate no valid direct connection.
        return numeric_limits<double>::infinity();
    }

    // Calculate the Euclidean distance between the two points.
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return sqrt((dx * dx) + (dy * dy));
}

void MST::readPoints() {
    size_t n;
    cin >> n;           // Read the number of points.
    points.resize(n);   // Resize the vector to hold the points.
    for (size_t i = 0; i < n; ++i) {
        cin >> points[i].x >> points[i].y;   // Read each point's x and y coordinates.
        // Determine the type based on coordinates, used for deciding connection validity.
        if (points[i].x == 0 || points[i].y == 0) {
            points[i].type = LocationType::Border;
        } else if (points[i].x < 0 && points[i].y < 0) {
            points[i].type = LocationType::MedicalCampus;
        } else {
            points[i].type = LocationType::MainCampus;
        }
    }
}

void MST::runMST() {
    size_t u = 0;   // To store the index of the vertex with the smallest edge.
    size_t n = points.size();
    minEdge.assign(n, numeric_limits<double>::infinity());
    prevVertex.assign(n, -1);
    visited.assign(n, false);

    minEdge[0] = 0;           // Initialize the edge cost of the first vertex to 0.
    double totalWeight = 0;   // To track the total weight of the MST.

    for (size_t i = 0; i < n; ++i) {
        // Reset the boolean for identifying smallest edge for each new vertex.
        bool b = false;
        for (size_t j = 0; j < n; ++j) {
            if (!visited[j] && (!b || minEdge[j] < minEdge[u])) {
                b = true;
                u = j;   // Find the unvisited vertex with the smallest edge.
            }
        }

        visited[u] = true;           // Mark the vertex as visited.
        totalWeight += minEdge[u];   // Add its weight to the total MST weight.

        // For each unvisited vertex, calculate the edge weight to vertex u.
        for (size_t v = 0; v < n; ++v) {
            // Ensure that we attempt to connect if either vertex is a border location
            if (!visited[v]
                && (points[u].type == LocationType::Border || points[v].type == LocationType::Border
                    || points[u].type == points[v].type)) {
                double weight = calculateDistance(u, v);
                if (weight < minEdge[v]) {
                    minEdge[v] = weight;                   // Update the smallest edge if current is smaller.
                    prevVertex[v] = static_cast<int>(u);   // Update the predecessor vertex.
                }
            }
        }
    }

    // Output the total weight of the MST.
    cout << fixed << totalWeight << "\n";
    // Output the edges in the MST.
    for (size_t v = 1; v < n; ++v) {
        if (prevVertex[v] != -1) {
            cout << min(prevVertex[v], static_cast<int>(v)) << " " << max(prevVertex[v], static_cast<int>(v)) << "\n";
        }
    }
}
