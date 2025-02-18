#ifndef CLASSIFY_HPP
#define CLASSIFY_HPP

#include <vector>

#include "graphTypes.hpp"

std::vector< std::pair<VertexID_t, int> > classify(ClusterMap& clusters, const std::vector<Expert>& experts, VertexMap& vertices);

#endif // CLASSIFY_HPP