#include "label.hpp"

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

const set<ClusterIDVertex> getVertices(const NNsupportEdges& se);

const LabeledVertices label(const NNsupportEdges& se, VertexMap& vertices, ClusterMap& clusters)
{
  LabeledVertices labeledVertices;
  const set<ClusterIDVertex> edgeVertices = getVertices(se);

  for (const auto& [vertexid, vertex] : vertices) {
    const auto farthest = max_element(edgeVertices.begin(), edgeVertices.end(),
      [&vertex](const auto& a, const auto& b) {
        return squaredDistance(vertex.features, a.second->features) < squaredDistance(vertex.features, b.second->features);
      });

    clusters[farthest->first].addVertex(vertexid, vertex);
    labeledVertices.push_back({vertexid, farthest->first});
  }

  return labeledVertices;
}

const set<ClusterIDVertex> getVertices(const NNsupportEdges& se)
{
  set<ClusterIDVertex> vertices;

  for (const auto& [vapair, vbpair] : se) {
    vertices.insert(vapair);
    vertices.insert(vbpair);
  }

  return vertices;
}
