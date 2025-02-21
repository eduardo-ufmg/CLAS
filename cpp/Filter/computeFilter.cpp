#include "computeFilter.hpp"

#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <iostream>

#include "graphTypes.hpp"

using namespace std;

void computeVertexQ(const shared_ptr<Vertex>& vertex, const Cluster& cluster);
void computeClusterQ(Cluster& cluster);
void computeClusterStats(Cluster& cluster);
void computeClusterThreshold(Cluster& cluster, double deviationFactor);
void filterVertices(Cluster& cluster);

void filterVertices(ClusterMap& clusters, double deviationFactor)
{
  for (auto& [_, cluster] : clusters) { (void)_;

    if (cluster.vertices.empty()) {
      continue;
    }

    for (auto& [_, vertex] : cluster.vertices) { (void)_;
      computeVertexQ(vertex, cluster);
    }

    computeClusterQ(cluster);

    computeClusterStats(cluster);

    computeClusterThreshold(cluster, deviationFactor);

    filterVertices(cluster);

  }
}

void computeVertexQ(const shared_ptr<Vertex>& vertex, const Cluster& cluster)
{
  unsigned degree = vertex->adjacents.size();
  unsigned sameClusterDegree = 0;

  if (degree == 0) {
    vertex->q = 0.0;
    return;
  }

  sameClusterDegree = count_if(vertex->adjacents.begin(), vertex->adjacents.end(),
  [&cluster](const auto& adjacent) {
    return cluster.vertices.find(adjacent.first) != cluster.vertices.end();
  });

  vertex->q = static_cast<double>(sameClusterDegree) / degree;
}

void computeClusterQ(Cluster& cluster)
{
  cluster.Q.magnitude = cluster.vertices.size();

  cluster.Q.sum_q = accumulate(cluster.vertices.begin(), cluster.vertices.end(), 0.0,
  [](double sum, const auto& pair) {
    return sum + pair.second->q;
  });
  
}

void computeClusterStats(Cluster& cluster)
{
  if (cluster.Q.magnitude == 0) {
    cluster.averageQuality = 0.0;
    cluster.stdDeviation = 0.0;
    return;
  }

  cluster.averageQuality = cluster.Q.sum_q / cluster.Q.magnitude;

  double sumSquaredDiff = transform_reduce(
    cluster.vertices.begin(), cluster.vertices.end(), 0.0,
    plus<>(),
    [averageQuality = cluster.averageQuality](const auto& pair) {
      double diff = pair.second->q - averageQuality;
      return diff * diff;
    }
  );

  cluster.stdDeviation = sqrt(sumSquaredDiff / cluster.Q.magnitude);
}

void computeClusterThreshold(Cluster& cluster, double deviationFactor)
{
  cluster.threshold = cluster.averageQuality - deviationFactor * cluster.stdDeviation;
}

void filterVertices(Cluster& cluster)
{
  erase_if(cluster.vertices, [&cluster](const auto& pair) {
    return pair.second->q < cluster.threshold;
  });
}
