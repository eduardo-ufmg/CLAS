#ifndef PRINTCONTAINERS_HPP
#define PRINTCONTAINERS_HPP

#include <iostream>

#include "types.hpp"

using namespace std;

void printVertices(const Vertices& vertices);
void printVertices(const Vertices& vertices, const size_t limitPerSegment);
void printVertexQuantity(const Vertices& vertices);
void printClusterStats(const Vertices& vertices);

#endif // PRINTCONTAINERS_HPP