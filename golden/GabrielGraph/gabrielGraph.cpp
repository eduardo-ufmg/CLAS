#include "gabrielGraph.hpp"

#include <cmath>
#include <vector>

#include "graphTypes.hpp"

using namespace std;

double squaredDistance(const std::vector<double>& a, const std::vector<double>& b);

void gabrielGraph(ClusterMap& clusters)
{
  // Collect all vertices from all clusters.
  VertexMap allVertices;

  for (auto& cluster : clusters) {
    Cluster& c = cluster.second;

    for (auto& vertex : c.vertices) {
      allVertices[vertex.first] = vertex.second;
    }

  }
  
  size_t N = allVertices.size();

  // For each unordered pair of vertices (pi, pj)
  for (size_t i = 0; i < N; ++i) {
    pair<VertexID_t, Vertex*> pi = *next(allVertices.begin(), i);

    for (size_t j = i + 1; j < N; ++j) {
      pair<VertexID_t, Vertex*> pj = *next(allVertices.begin(), j);

      double d2 = squaredDistance(pi.second->features, pj.second->features);
      bool validEdge = true;

      // Check the Gabriel condition against every other vertex z.
      for (size_t k = 0; k < N; ++k) {
        if (k == i || k == j) {
          continue;
        }

        Vertex* pk = allVertices[k];
        
        // If for any other vertex the sum of squared distances is less than d2,
        // then the sphere with (pi, pj) as diameter contains pk.
        if (squaredDistance(pi.second->features, pk->features) + 
          squaredDistance(pj.second->features, pk->features) < d2) {
          validEdge = false;
          break;
        }

      }

      // If the edge passes the test, add each vertex to the other's adjacency list.
      if (validEdge) {
        pi.second->adjacents.push_back(pj.first);
        pj.second->adjacents.push_back(pi.first);
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