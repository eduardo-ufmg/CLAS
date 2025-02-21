#include "classify.hpp"

#include <vector>
#include <map>
#include <cmath>
#include <iostream>
#include <numeric>
#include <algorithm>

#include "graphTypes.hpp"
#include "getSEs.hpp"
#include "squaredDistance.hpp"

using namespace std;

set< pair< ClusterID, const Vertex* > > getVertices(const NNsupportEdges& se);
ClassifiedVertices classify(const NNsupportEdges& se, VertexMap& vertices, ClusterMap& clusters)
{
  ClassifiedVertices classifiedVertices;
  auto edgeVertices = getVertices(se);

  for (auto& [vertexid, vertex] : vertices) {
    ClusterID farthestVertexClusterid;
    double farthestDistance = numeric_limits<double>::min();

    for (const auto& [clusterid, edgeVertex] : edgeVertices) {
      const double distance = squaredDistance(vertex->features, edgeVertex->features);

      if (distance > farthestDistance) {
        farthestDistance = distance;
        farthestVertexClusterid = clusterid;
      }
    }

    clusters[farthestVertexClusterid].addVertex(vertexid, vertex);
    classifiedVertices.push_back({vertexid, farthestVertexClusterid});
  }

  return classifiedVertices;
}

set< pair< ClusterID, const Vertex* > > getVertices(const NNsupportEdges& se)
{
  set< pair< ClusterID, const Vertex* > > vertices;

  for (auto& [vapair, vbpair] : se) {
    vertices.insert(vapair);
    vertices.insert(vbpair);
  }

  return vertices;
}
