#include "computeExperts.hpp"

#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <stdexcept>
#include <numeric>

#include "graphTypes.hpp"

using namespace std;

shared_ptr<Vertex> findVertex(const VertexID vertexid, const Cluster& cluster);
EdgeVertices getEdgeVertices(const Edge& edge, const ClusterMap& clusters);
vector<double> computeDifferences(const EdgeVertices& vertices);
vector<double> computeMidpoint(const EdgeVertices& vertices);
double computeBias(const vector<double>& differences, const vector<double>& midpoint);
Expert createExpert(const Edge& edge, const ClusterMap& clusters, const unsigned expertid);

SupportEdges getSEs(const ClusterMap& clusters)
{
  SupportEdges supportEdges;

  for (const auto& [_, cluster] : clusters) { (void)_;
    for (const auto& [vertexid, vertex] : cluster.vertices) {
      for (const auto& [adjacentID, isSE] : vertex->adjacents) {

        if (isSE) {
          supportEdges.insert(make_pair(min(vertexid, adjacentID), max(vertexid, adjacentID)));
        }

      }
    }
  }

  return supportEdges;
}

vector<Expert> getExperts(const SupportEdges& supportEdges, const ClusterMap& clusters)
{
  vector<Expert> experts(supportEdges.size());
  unsigned expertid = 0;

  transform(supportEdges.begin(), supportEdges.end(), experts.begin(),
    [&clusters, &expertid](const Edge& edge) {
      return createExpert(edge, clusters, expertid++);
    });

  return experts;
}

shared_ptr<Vertex> findVertex(const VertexID vertexid, const Cluster& cluster)
{
  auto vertex = cluster.vertices.find(vertexid);
  return vertex != cluster.vertices.end() ? vertex->second : nullptr;
}

EdgeVertices getEdgeVertices(const Edge& edge, const ClusterMap& clusters)
{
  const Vertex* v0 = nullptr;
  const Vertex* v1 = nullptr;

  find_if(clusters.begin(), clusters.end(),
    [&v0, &v1, &edge](const auto& clusterpair) {
      const auto& cluster = clusterpair.second;
      if (!v0) v0 = findVertex(edge.first, cluster).get();
      if (!v1) v1 = findVertex(edge.second, cluster).get();
      return v0 && v1;
    });

  if (!v0 || !v1) {
    throw invalid_argument("Edge vertices not found in clusters");
  }

  return make_pair(v0, v1);
}

vector<double> computeDifferences(const EdgeVertices& vertices)
{
  vector<double> differences(vertices.first->features.size());

  transform(vertices.first->features.begin(), vertices.first->features.end(),
    vertices.second->features.begin(), differences.begin(),
    minus<double>());

  return differences;
}

vector<double> computeMidpoint(const EdgeVertices& vertices)
{
  vector<double> midpoint(vertices.first->features.size());

  transform(vertices.first->features.begin(), vertices.first->features.end(),
    vertices.second->features.begin(), midpoint.begin(),
    [](double a, double b) {
      return (a + b) / 2;
    });

  return midpoint;
}

double computeBias(const vector<double>& differences, const vector<double>& midpoint)
{
  return inner_product(differences.begin(), differences.end(), midpoint.begin(), 0.0);
}

Expert createExpert(const Edge& edge, const ClusterMap& clusters, const unsigned expertid)
{
  Expert expert;
  EdgeVertices vertices = getEdgeVertices(edge, clusters);

  expert.edge = edge;
  expert.differences = computeDifferences(vertices);
  expert.midpoint_coordinates = computeMidpoint(vertices);
  expert.bias = computeBias(expert.differences, expert.midpoint_coordinates);
  expert.id = expertid;

  return expert;
}
