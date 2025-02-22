#include "computeExperts.hpp"

#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <stdexcept>
#include <numeric>

#include "graphTypes.hpp"

using namespace std;

Vertex* findVertex(VertexID vertexid, Cluster cluster);
EdgeVertices getEdgeVertices(Edge edge, ClusterMap clusters);
vector<double> computeDifferences(EdgeVertices vertices);
vector<double> computeMidpoint(EdgeVertices vertices);
double computeBias(vector<double> differences, vector<double> midpoint);
Expert createExpert(Edge edge, ClusterMap clusters, unsigned expertid);

SupportEdges getSEs(ClusterMap clusters)
{
  SupportEdges supportEdges;

  for (auto [_, cluster] : clusters) { (void)_;
    for (auto [vertexid, vertex] : cluster.vertices) {
      for (auto [adjacentid, isSE] : vertex.adjacents) {

        if (isSE) {
          supportEdges.insert(make_pair(min(vertexid, adjacentid), max(vertexid, adjacentid)));
        }

      }
    }
  }

  return supportEdges;
}

vector<Expert> getExperts(SupportEdges supportEdges, ClusterMap clusters)
{
  vector<Expert> experts(supportEdges.size());
  unsigned expertid = 0;

  transform(supportEdges.begin(), supportEdges.end(), experts.begin(),
    [clusters, &expertid](Edge edge) {
      return createExpert(edge, clusters, expertid++);
    });

  return experts;
}

Vertex* findVertex(VertexID vertexid, Cluster cluster)
{
  auto vertex = cluster.vertices.find(vertexid);
  return vertex != cluster.vertices.end() ? &(vertex->second) : nullptr;
}

EdgeVertices getEdgeVertices(Edge edge, ClusterMap clusters)
{
  Vertex* v0 = nullptr;
  Vertex* v1 = nullptr;

  find_if(clusters.begin(), clusters.end(),
    [&v0, &v1, edge](auto clusterpair) {
      auto cluster = clusterpair.second;
      if (!v0) v0 = findVertex(edge.first, cluster);
      if (!v1) v1 = findVertex(edge.second, cluster);
      return v0 && v1;
    });

  if (!v0 || !v1) {
    throw invalid_argument("Edge vertices not found in clusters");
  }

  return make_pair(v0, v1);
}

vector<double> computeDifferences(EdgeVertices vertices)
{
  vector<double> differences(vertices.first->features.size());

  transform(vertices.first->features.begin(), vertices.first->features.end(),
    vertices.second->features.begin(), differences.begin(),
    minus<double>());

  return differences;
}

vector<double> computeMidpoint(EdgeVertices vertices)
{
  vector<double> midpoint(vertices.first->features.size());

  transform(vertices.first->features.begin(), vertices.first->features.end(),
    vertices.second->features.begin(), midpoint.begin(),
    [](double a, double b) {
      return (a + b) / 2;
    });

  return midpoint;
}

double computeBias(vector<double> differences, vector<double> midpoint)
{
  return inner_product(differences.begin(), differences.end(), midpoint.begin(), 0.0);
}

Expert createExpert(Edge edge, ClusterMap clusters, unsigned expertid)
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
