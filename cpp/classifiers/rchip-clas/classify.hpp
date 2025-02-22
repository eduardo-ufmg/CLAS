#ifndef CLASSIFY_HPP
#define CLASSIFY_HPP

#include <vector>

#include "graphTypes.hpp"

ClassifiedVertices classify(ClusterMap clusters, std::vector<Expert> experts, VertexMap vertices);

#endif // CLASSIFY_HPP