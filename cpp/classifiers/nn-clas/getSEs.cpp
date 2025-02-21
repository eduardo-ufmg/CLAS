#include "getSEs.hpp"

#include <set>

#include "graphTypes.hpp"

using namespace std;



NNsupportEdges getSEs(const ClusterMap& clusters)
{
  using VertexClusterIDMap = map< VertexID, pair<ClusterID, const Vertex*> >;

  VertexClusterIDMap vertices = [&clusters]() -> VertexClusterIDMap {
    VertexClusterIDMap vertices;
    for (const auto& [clusterid, cluster] : clusters) {
      for (const auto& [vertexid, vertex] : cluster.vertices) {
        vertices.emplace(vertexid, make_pair(clusterid, vertex.get()));
      }
    }
    return vertices;
  }();

  NNsupportEdges supportEdges;

  for (const auto& [vertexid, vertexpair] : vertices) {
    const auto& [clusterid, vertex] = vertexpair;
    for (const auto& [adjacentid, isSE] : vertex->adjacents) {
      if (isSE) {
        const auto& vpair = make_pair(clusterid, vertex);
        const auto& adjpair = vertices.at(adjacentid);

        const auto& edge = make_pair(min(vpair, adjpair), max(vpair, adjpair));

        supportEdges.insert(edge);
      }
    }
  }

  return supportEdges;
}
