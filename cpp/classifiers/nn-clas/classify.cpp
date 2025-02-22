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

set< pair< ClusterID, Vertex* > > getVertices(NNsupportEdges se);

ClassifiedVertices classify(NNsupportEdges se, VertexMap vertices, ClusterMap clusters)
{
  ClassifiedVertices classifiedVertices;
  auto edgeVertices = getVertices(se);

  for (auto [vertexid, vertex] : vertices) {
    auto farthest = max_element(edgeVertices.begin(), edgeVertices.end(),
      [vertex](auto a, auto b) {
        return squaredDistance(vertex.features, a.second->features) < squaredDistance(vertex.features, b.second->features);
      });

    clusters[farthest->first].addVertex(vertexid, vertex);
    classifiedVertices.push_back({vertexid, farthest->first});
  }

  return classifiedVertices;
}

set< pair< ClusterID, Vertex* > > getVertices(NNsupportEdges se)
{
  set< pair< ClusterID, Vertex* > > vertices;

  for (auto [vapair, vbpair] : se) {
    vertices.insert(vapair);
    vertices.insert(vbpair);
  }

  return vertices;
}
