#ifndef BUILD_GRAPH_HPP
#define BUILD_GRAPH_HPP

#include <vector>

#include "graphTypes.hpp"

void buildVertexVector(std::vector<Vertex>& vertices, const std::vector< std::vector<double> >& raw_vertices, const std::vector< std::pair<int,int> >& raw_edges);
void buildClusters(std::vector<Cluster>& clusters, const std::vector<Vertex>& all_vertices, const std::vector<int>& raw_targets);

#endif // BUILD_GRAPH_HPP