#include "gabrielGraph.hpp"

#include <cmath>
#include <vector>

#include "graphTypes.hpp"

using namespace std;

double squaredDistance(const std::vector<double>& a, const std::vector<double>& b);

void gabrielGraph(ClusterMap& clusters)
{
  // Collect all vertices from all clusters.
  std::vector<Vertex*> allVertices;

  for (auto& clusterPair : clusters) {
    Cluster& cluster = clusterPair.second;

    for (auto& vertex : cluster.vertices) {
      allVertices.push_back(&vertex);
    }

  }
  
  size_t N = allVertices.size();

  // For each unordered pair of vertices (pi, pj)
  for (size_t i = 0; i < N; ++i) {
    Vertex* pi = allVertices[i];

    for (size_t j = i + 1; j < N; ++j) {
      Vertex* pj = allVertices[j];
      double d2 = squaredDistance(pi->features, pj->features);
      bool validEdge = true;

      // Check the Gabriel condition against every other vertex z.
      for (size_t k = 0; k < N; ++k) {
        if (k == i || k == j) {
          continue;
        }

        Vertex* pk = allVertices[k];
        
        // If for any other vertex the sum of squared distances is less than d2,
        // then the sphere with (pi, pj) as diameter contains pk.
        if (squaredDistance(pi->features, pk->features) + 
          squaredDistance(pj->features, pk->features) < d2) {
          validEdge = false;
          break;
        }

      }

      // If the edge passes the test, add each vertex to the other's adjacency list.
      if (validEdge) {
        pi->adjacents.push_back(pj->id);
        pj->adjacents.push_back(pi->id);
      }

    }

  }
}

double squaredDistance(const std::vector<double>& a, const std::vector<double>& b)
{
  double sum = 0.0;
  size_t n = a.size();

  for (size_t i = 0; i < n; ++i) {
    double diff = a[i] - b[i];
    sum += diff * diff;
  }

  return sum;
}