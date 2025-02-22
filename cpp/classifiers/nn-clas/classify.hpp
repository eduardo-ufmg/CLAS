#ifndef CLASSIFY_HPP
#define CLASSIFY_HPP

#include "graphTypes.hpp"
#include "getSEs.hpp"

const ClassifiedVertices classify(const NNsupportEdges& se, VertexMap& vertices, ClusterMap& clusters);

#endif // CLASSIFY_HPP