#ifndef COMPUTEEXPERTS_HPP
#define COMPUTEEXPERTS_HPP

#include <vector>

#include "graphTypes.hpp"

SupportEdges getSEs(const ClusterMap& clusters);
std::vector<Expert> getExperts(const SupportEdges& supportEdges, const ClusterMap& clusters);

#endif // COMPUTEEXPERTS_HPP