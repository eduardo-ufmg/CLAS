#ifndef COMPUTEEXPERTS_HPP
#define COMPUTEEXPERTS_HPP

#include <vector>

#include "graphTypes.hpp"

const SupportEdges getSEs(const ClusterMap& clusters);
const Experts getExperts(const SupportEdges& supportEdges, ClusterMap& clusters);

#endif // COMPUTEEXPERTS_HPP