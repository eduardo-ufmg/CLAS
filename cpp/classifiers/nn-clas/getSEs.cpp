#include "getSEs.hpp"

#include <set>

#include "graphTypes.hpp"

using namespace std;



NNsupportEdges getSEs(ClusterMap clusters)
{
  using VertexClusterIDMap = map< VertexID, pair<ClusterID, Vertex*> >;

  VertexClusterIDMap vertices = [clusters]() -> VertexClusterIDMap {
    VertexClusterIDMap vertices;
    for (auto [clusterid, cluster] : clusters) {
      for (auto [vertexid, vertex] : cluster.vertices) {
        vertices.emplace(vertexid, make_pair(clusterid, vertex.get()));
      }
    }
    return vertices;
  }();

  NNsupportEdges supportEdges;

  for (auto [vertexid, vertexpair] : vertices) {
    auto [clusterid, vertex] = vertexpair;
    for (auto [adjacentid, isSE] : vertex->adjacents) {
      if (isSE) {
        auto vpair = make_pair(clusterid, vertex);
        auto adjpair = vertices.at(adjacentid);

        auto edge = make_pair(min(vpair, adjpair), max(vpair, adjpair));

        supportEdges.insert(edge);
      }
    }
  }

  return supportEdges;
}
