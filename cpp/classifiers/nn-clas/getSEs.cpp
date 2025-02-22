#include "getSEs.hpp"

#include <set>

#include "graphTypes.hpp"

using namespace std;

const NNsupportEdges getSEs(const ClusterMap& clusters)
{
  using VertexClusterIDMap = map< VertexID, ClusterIDVertex >;

  const VertexClusterIDMap vertices = [&clusters]() -> const VertexClusterIDMap {
    VertexClusterIDMap vertices;
    for (const auto& [clusterid, cluster] : clusters) {
      for (const auto& [vertexid, vertex] : cluster.vertices) {
        vertices.emplace(vertexid, make_pair(clusterid, &vertex));
      }
    }
    return vertices;
  }();

  NNsupportEdges supportEdges;

  for (const auto& [vertexid, vertexpair] : vertices) {
    for (auto [adjacentid, isSE] : vertexpair.second->adjacents) {
      if (isSE) {
        const auto& adjpair = vertices.at(adjacentid);

        NNedge edge = make_pair(min(vertexpair, adjpair), max(vertexpair, adjpair));

        supportEdges.insert(edge);
      }
    }
  }

  return supportEdges;
}
