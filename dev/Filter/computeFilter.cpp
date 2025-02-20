#include "computeFilter.hpp"

#include <vector>
#include <map>
#include <cmath>
#include <set>
#include <algorithm>
#include <numeric>
#include <iostream>

#include "graphTypes.hpp"

using namespace std;

void filterVertices(ClusterMap& clusters, double deviationFactor)
{
  for (auto& [_, cluster] : clusters) {
    (void)_;

    if (cluster.vertices.empty()) {
      continue;
    }

    for (auto& vertex : cluster.vertices) {
      unsigned degree = vertex.second->adjacents.size();
      unsigned sameClusterDegree = 0;

      if (degree == 0) {
        vertex.second->q = 0.0;
        continue;
      }

      for (const auto& adjacent : vertex.second->adjacents) {
        if (cluster.vertices.find(adjacent.first) != cluster.vertices.end()) {
          ++sameClusterDegree;
        }
      }

      vertex.second->q = static_cast<double>(sameClusterDegree) / degree;

    }

    // Recompute quality metrics for the cluster.
    cluster.Q.magnitude = cluster.vertices.size();

    // Compute the sum of q values
    cluster.Q.sum_q = 0.0;
    for (const auto& v : cluster.vertices) {
      cluster.Q.sum_q += v.second->q;
    }

    cluster.averageQuality = cluster.Q.sum_q / cluster.Q.magnitude;

    double sumSquaredDiff = 0.0;

    for (const auto& v : cluster.vertices) {
      double diff = v.second->q - cluster.averageQuality;
      sumSquaredDiff += diff * diff;
    }

    cluster.stdDeviation = sqrt(sumSquaredDiff / cluster.Q.magnitude);

    // Update the threshold using the deviationFactor.
    cluster.threshold = cluster.averageQuality - deviationFactor * cluster.stdDeviation;

    // Remove vertices whose q value is less than the threshold.
    for (auto it = cluster.vertices.begin(); it != cluster.vertices.end();) {
      if (it->second->q < cluster.threshold) {
        it = cluster.vertices.erase(it);
      } else {
        ++it;
      }
    }

  }
}
