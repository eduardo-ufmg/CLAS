#ifndef GETSE_HPP
#define GETSE_HPP

#include "graphTypes.hpp"

using NNedge = std::pair< std::pair<ClusterID, const Vertex*>, std::pair<ClusterID, const Vertex*> >;
using NNsupportEdges = std::set<NNedge>;

NNsupportEdges getSEs(const ClusterMap& clusters);

#endif // GETSE_HPP
