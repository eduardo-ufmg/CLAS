#ifndef COMPUTEEXPERTS_HPP
#define COMPUTEEXPERTS_HPP

#include <vector>

#include "graphTypes.hpp"

SupportEdges getSEs(ClusterMap clusters);
std::vector<Expert> getExperts(SupportEdges supportEdges, ClusterMap clusters);

#endif // COMPUTEEXPERTS_HPP