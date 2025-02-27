#include "types.hpp"

#include <cmath>
#include <algorithm>
#include <numeric>

using namespace std;

Vertex::Vertex(const VertexID id, const Coordinates coordinates, const Cluster * const cluster)
  : id(id), coordinates(coordinates), cluster(cluster), quality(0.0f)
{}

Cluster::Cluster(const ClusterID id)
  : id(id), sumq(0.0f), magq(0), online_avgq(0.0f), sumDeltaSq(0.0f), online_stdq(0.0f), treshold(0.0f)
{}

void Cluster::reset()
{
  sumq = 0.0f;
  magq = 0;
  online_avgq = 0.0f;
  sumDeltaSq = 0.0f;
  online_stdq = 0.0f;
  treshold = 0.0f;
}

void Cluster::accumQ_updateStats(const float q)
{
  sumq += q;
  ++ magq;

  const float delta = q - online_avgq;

  online_avgq += delta / magq;

  const float delta2 = delta - online_avgq;

  sumDeltaSq += delta * delta2;

  if (magq > 1) {
    online_stdq = sqrt(sumDeltaSq / (magq - 1));
  }
}

void Cluster::computeTreshold(const float tolerance)
{
  treshold = online_avgq - tolerance * online_stdq;
}

const Coordinates Expert::computeMidpoint(const Edge& edge)
{
  const auto& [v1, v2] = edge;
  const auto& c1 = v1->coordinates;
  const auto& c2 = v2->coordinates;

  Coordinates midpoint(c1.size());

  transform(c1.begin(), c1.end(),
            c2.begin(),
            midpoint.begin(),
            [](const float x, const float y) {
              return (x + y) / 2.0f;
            });

  return midpoint;
}

const ExpertDifferences Expert::computeDifferences(const Edge& edge)
{
  const auto& [v1, v2] = edge;
  const auto& c1 = v1->coordinates;
  const auto& c2 = v2->coordinates;

  ExpertDifferences differences(c1.size());

  transform(c1.begin(), c1.end(),
            c2.begin(),
            differences.begin(),
            minus<float>());

  return differences;
}

float Expert::computeBias(const Coordinates& midpoint, const ExpertDifferences& differences)
{
  const float sum = inner_product(differences.begin(), differences.end(),
                                  midpoint.begin(),
                                  0.0f,
                                  plus<float>(),
                                  [](const float x, const float y) {
                                    return x * y;
                                  });

  return sum / midpoint.size();
}

Expert::Expert(const ExpertID id, const Edge edge)
  : id(id), edge(edge),
  midpoint(computeMidpoint(edge)),
  differences(computeDifferences(edge)),
  bias(computeBias(midpoint, differences))
{}