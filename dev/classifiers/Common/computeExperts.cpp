#include "computeExperts.hpp"

#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <stdexcept>

#include "graphTypes.hpp"

using namespace std;

EdgeVertices getEdgeVertices(const Edge& edge, const ClusterMap& clusters);
vector<double> computeDifferences(const EdgeVertices& vertices);
vector<double> computeMidpoint(const EdgeVertices& vertices);
double computeBias(const vector<double>& differences, const vector<double>& midpoint);

SupportEdges getSEs(const ClusterMap& clusters)
{
  SupportEdges supportEdges;

  for (const auto& [_, cluster] : clusters) { (void)_;
    for (const auto& [vertexid, vertex] : cluster.vertices) {
      for (const auto& [adjacentID, isSE] : vertex->adjacents) {
        Edge orderedEdge = make_pair(min(vertexid, adjacentID), max(vertexid, adjacentID));

        if (isSE && supportEdges.find(orderedEdge) == supportEdges.end()) {
          supportEdges.insert(orderedEdge);
        }

      }
    }
  }

  return supportEdges;
}

vector<Expert> getExperts(const SupportEdges& supportEdges, const ClusterMap& clusters)
{
  vector<Expert> experts;
  unsigned expertID = 0;

  for (const auto& edge : supportEdges) {
    Expert expert;
    EdgeVertices vertices = getEdgeVertices(edge, clusters);

    expert.edge = edge;
    expert.differences = computeDifferences(vertices);
    expert.midpoint_coordinates = computeMidpoint(vertices);
    expert.bias = computeBias(expert.differences, expert.midpoint_coordinates);
    expert.id = expertID++;

    experts.push_back(expert);
  }

  return experts;
}

EdgeVertices getEdgeVertices(const Edge& edge, const ClusterMap& clusters)
{
  const Vertex* v0;
  const Vertex* v1;

  bool foundV0 = false;
  bool foundV1 = false;

  for (const auto& [_, cluster] : clusters) { (void)_;
    if (!foundV0 && cluster.vertices.find(edge.first) != cluster.vertices.end()) {
      v0 = cluster.vertices.at(edge.first).get();
      foundV0 = true;
    }

    if (!foundV1 && cluster.vertices.find(edge.second) != cluster.vertices.end()) {
      v1 = cluster.vertices.at(edge.second).get();
      foundV1 = true;
    }
  }

  if (!foundV0 || !foundV1) {
    throw runtime_error("Edge not found in clusters");
  }

  return make_pair(v0, v1);
}

vector<double> computeDifferences(const EdgeVertices& vertices)
{
  vector<double> differences;

  for (size_t i = 0; i < vertices.first->features.size(); ++i) {
    differences.push_back(vertices.first->features[i] - vertices.second->features[i]);
  }

  return differences;
}

vector<double> computeMidpoint(const EdgeVertices& vertices)
{
  vector<double> midpoint;

  for (size_t i = 0; i < vertices.first->features.size(); ++i) {
    midpoint.push_back((vertices.first->features[i] + vertices.second->features[i]) / 2);
  }

  return midpoint;
}

double computeBias(const vector<double>& differences, const vector<double>& midpoint)
{
  double bias = 0;

  for (size_t i = 0; i < differences.size(); ++i) {
    bias += differences[i] * midpoint[i];
  }

  return bias;
}
