#ifndef PRINTLOADEDDATA_HPP
#define PRINTLOADEDDATA_HPP

#include <iostream>

#include "graphTypes.hpp"

void printClusterVertexQuantity(const ClusterMap& clusters);
void printClusterStatistics(const ClusterMap& clusters);
void printAdjacencyLists(const ClusterMap& clusters, int maxCluster, int maxVertexPerCluster);

#endif // PRINTLOADEDDATA_HPP
