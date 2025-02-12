#include "filter.hpp"

#include <vector>
#include <map>
#include <cmath>
#include <set>
#include <algorithm>

#include "graphTypes.hpp"

using namespace std;

unsigned countAdjacentsInSameCluster(const vector<ID_t>& adjacents, const vector<Vertex>& vertices);

void computeVerticesAndClusterQuality(vector<Cluster>& clusters)
{
  for (auto& cluster : clusters) {
    cluster.Q.sum_q = 0;
    cluster.Q.magnitude = 0;

    for (auto& vertex : cluster.vertices) {
      unsigned vertex_degree = vertex.adjacents.size();
      unsigned vertex_same_cluster_degree = countAdjacentsInSameCluster(vertex.adjacents, cluster.vertices);

      vertex.q = static_cast<double>(vertex_same_cluster_degree) / static_cast<double>(vertex_degree);

      cluster.Q.sum_q += vertex.q;
      cluster.Q.magnitude += 1;
    }

    cluster.averageQuality = cluster.Q.sum_q / cluster.Q.magnitude;
  }
}

void computeClusterDeviation(vector<Cluster>& clusters)
{
  for (auto& cluster : clusters) {
    cluster.stdDeviation = 0;

    for (auto& vertex : cluster.vertices) {
      cluster.stdDeviation += pow(vertex.q - cluster.averageQuality, 2);
    }

    cluster.stdDeviation = sqrt(cluster.stdDeviation / cluster.Q.magnitude);
  }
}

void computeClusterTresholds(vector<Cluster>& clusters, double deviation_factor)
{
  for (auto& cluster : clusters) {
    cluster.treshold = cluster.averageQuality - deviation_factor * cluster.stdDeviation;
  }
}

void filterVertices(vector<Cluster>& clusters)
{
  vector<ID_t> removedVertices;

  for (auto& cluster : clusters) {
    for (auto vertex_it = cluster.vertices.begin(); vertex_it != cluster.vertices.end(); ) {
      if (vertex_it->q < cluster.treshold) {
        removedVertices.push_back(vertex_it->id);
        vertex_it = cluster.vertices.erase(vertex_it);
      } else {
        vertex_it ++;
      }
    }
  }

  for (auto& cluster : clusters) {
    for (auto& vertex : cluster.vertices) {
      for (auto& adjacent : vertex.adjacents) {
        for (auto& removedVertex : removedVertices) {
          if (adjacent == removedVertex) {
            vertex.adjacents.erase(find(vertex.adjacents.begin(), vertex.adjacents.end(), adjacent));
            break;
          }
        }
      }
    }
  }
}

unsigned countAdjacentsInSameCluster(const vector<ID_t>& adjacents, const vector<Vertex>& vertices)
{
  unsigned count = 0;

  for (auto& adjacent : adjacents) {
    for (auto& vertex : vertices) {
      if (vertex.id == adjacent) {
        count++;
        break;
      }
    }
  }
  
  return count;
}
