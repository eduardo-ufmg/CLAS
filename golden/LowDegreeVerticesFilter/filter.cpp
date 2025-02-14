#include "filter.hpp"

#include <vector>
#include <map>
#include <cmath>
#include <set>
#include <algorithm>
#include <numeric>

#include "graphTypes.hpp"

using namespace std;

void filterVertices(ClusterMap& clusters, double deviationFactor)
{
  for (auto& [key, cluster] : clusters) {
    // Avoid division by zero if there are no vertices.
    if (cluster.vertices.empty()) {
      continue;
    }

    for (auto& vertex : cluster.vertices) {
      unsigned degree = vertex.adjacents.size();
      unsigned sameClusterDegree = 0;

      if (degree == 0) {
        vertex.q = 0.0;
        continue;
      }

      for (const auto& adjacent : vertex.adjacents) {
        if (std::find(cluster.vertices.begin(), cluster.vertices.end(), *adjacent) != cluster.vertices.end()) {
          sameClusterDegree ++;
        }
      }

      vertex.q = static_cast<double>(sameClusterDegree) / degree;

    }

    // Recompute quality metrics for the cluster.
    cluster.Q.magnitude = cluster.vertices.size();

    cluster.Q.sum_q = std::accumulate(cluster.vertices.begin(), cluster.vertices.end(), 0.0, [](double sum, const Vertex& v) { return sum + v.q; });

    cluster.averageQuality = cluster.Q.sum_q / cluster.Q.magnitude;

    double sumSquaredDiff = 0.0;

    for (const Vertex& v : cluster.vertices) {
      double diff = v.q - cluster.averageQuality;
      sumSquaredDiff += diff * diff;
    }

    cluster.stdDeviation = std::sqrt(sumSquaredDiff / cluster.Q.magnitude);

    // Update the threshold using the deviationFactor.
    cluster.threshold = cluster.averageQuality - deviationFactor * cluster.stdDeviation;

    // Remove vertices whose q value is less than the threshold.
    cluster.vertices.erase(std::remove_if(cluster.vertices.begin(), cluster.vertices.end(), [&cluster](const Vertex& v) { return v.q < cluster.threshold; }), cluster.vertices.end());

  }
}

