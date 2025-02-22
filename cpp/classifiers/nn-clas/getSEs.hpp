#ifndef GETSE_HPP
#define GETSE_HPP

#include "graphTypes.hpp"

using ClusterIDVertex = std::pair<const ClusterID, const Vertex * const>;
using NNedge = std::pair< ClusterIDVertex, ClusterIDVertex >;
using NNsupportEdges = std::set<NNedge>;

const NNsupportEdges getSEs(const ClusterMap& clusters);

#endif // GETSE_HPP
