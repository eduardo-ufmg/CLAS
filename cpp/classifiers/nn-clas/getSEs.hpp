#ifndef GETSE_HPP
#define GETSE_HPP

#include "graphTypes.hpp"

using NNedge = std::pair< std::pair<ClusterID, Vertex*>, std::pair<ClusterID, Vertex*> >;
using NNsupportEdges = std::set<NNedge>;

NNsupportEdges getSEs(ClusterMap clusters);

#endif // GETSE_HPP
