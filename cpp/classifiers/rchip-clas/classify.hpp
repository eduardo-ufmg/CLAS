#ifndef CLASSIFY_HPP
#define CLASSIFY_HPP

#include <vector>

#include "graphTypes.hpp"

const ClassifiedVertices classify(ClusterMap& clusters, Experts& experts, VertexMap& vertices);

#endif // CLASSIFY_HPP