#include "computeExperts.hpp"

#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <stdexcept>
#include <numeric>

#include "graphTypes.hpp"

using namespace std;

Vertex* findVertex(const VertexID vertexid, Cluster& cluster);
const EdgeVertices getEdgeVertices(const Edge& edge, ClusterMap& clusters);
const ExpertDifferences computeDifferences(const EdgeVertices& vertices);
const Coordinates computeMidpoint(const EdgeVertices& vertices);
double computeBias(const ExpertDifferences& differences, const Coordinates& midpoint);
const Expert createExpert(const Edge& edge, ClusterMap& clusters, const ExpertID expertid);

const SupportEdges getSEs(const ClusterMap& clusters)
{
  SupportEdges supportEdges;

  for (const auto& [_, cluster] : clusters) { (void)_;
    for (const auto& [vertexid, vertex] : cluster.vertices) {
      for (const auto& [adjacentid, isSE] : vertex.adjacents) {

        if (isSE) {
          supportEdges.insert(make_pair(min(vertexid, adjacentid), max(vertexid, adjacentid)));
        }

      }
    }
  }

  return supportEdges;
}

const Experts getExperts(const SupportEdges& supportEdges, ClusterMap& clusters)
{
  Experts experts;
  ExpertID expertid = 0;

  experts.reserve(supportEdges.size());

  for (const auto& edge : supportEdges) {
    experts.push_back(createExpert(edge, clusters, expertid++));
  }

  return experts;
}

Vertex* findVertex(const VertexID vertexid, Cluster& cluster)
{
  const auto& it = cluster.vertices.find(vertexid);

  if (it == cluster.vertices.end()) {
    return nullptr;
  }

  return &(it->second);
}

const EdgeVertices getEdgeVertices(const Edge& edge, ClusterMap& clusters)
{
  Vertex* v0 = nullptr;
  Vertex* v1 = nullptr;

  find_if(clusters.begin(), clusters.end(),
    [&v0, &v1, &edge](auto& clusterpair) {
      auto& cluster = clusterpair.second;
      if (!v0) v0 = findVertex(edge.first, cluster);
      if (!v1) v1 = findVertex(edge.second, cluster);
      return v0 && v1;
    });

  if (!v0 || !v1) {
    throw invalid_argument("Edge vertices not found in clusters");
  }

  return make_pair(v0, v1);
}

const ExpertDifferences computeDifferences(const EdgeVertices& vertices)
{
  ExpertDifferences differences(vertices.first->features.size());

  transform(vertices.first->features.begin(), vertices.first->features.end(),
    vertices.second->features.begin(), differences.begin(),
    minus<double>());

  return differences;
}

const Coordinates computeMidpoint(const EdgeVertices& vertices)
{
  Coordinates midpoint(vertices.first->features.size());

  transform(vertices.first->features.begin(), vertices.first->features.end(),
    vertices.second->features.begin(), midpoint.begin(),
    [](const double a, const double b) {
      return (a + b) / 2;
    });

  return midpoint;
}

double computeBias(const ExpertDifferences& differences, const Coordinates& midpoint)
{
  return inner_product(differences.begin(), differences.end(), midpoint.begin(), 0.0);
}

const Expert createExpert(const Edge& edge, ClusterMap& clusters, const ExpertID expertid)
{
  const EdgeVertices vertices = getEdgeVertices(edge, clusters);
  const ExpertDifferences differences = computeDifferences(vertices);
  const Coordinates midpoint = computeMidpoint(vertices);
  const double bias = computeBias(differences, midpoint);

  return Expert(edge, differences, midpoint, bias, expertid);
}
