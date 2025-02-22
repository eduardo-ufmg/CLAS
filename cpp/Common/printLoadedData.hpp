#ifndef PRINTLOADEDDATA_HPP
#define PRINTLOADEDDATA_HPP

#include <iostream>

#include "graphTypes.hpp"

void printClusterVertexQuantity(const ClusterMap& clusters);
void printClusterStatistics(const ClusterMap& clusters);
void printAdjacencyLists(const ClusterMap& clusters, const size_t maxCluster, const size_t maxVertexPerCluster);
void printExpertData(const Experts& experts);

#endif // PRINTLOADEDDATA_HPP
