#ifndef PRINTLOADEDDATA_HPP
#define PRINTLOADEDDATA_HPP

#include <iostream>

#include "graphTypes.hpp"

void printClusterVertexQuantity(ClusterMap clusters);
void printClusterStatistics(ClusterMap clusters);
void printAdjacencyLists(ClusterMap clusters, int maxCluster, int maxVertexPerCluster);
void printExpertData(std::vector<Expert> experts);

#endif // PRINTLOADEDDATA_HPP
