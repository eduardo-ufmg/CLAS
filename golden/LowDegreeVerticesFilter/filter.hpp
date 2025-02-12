#ifndef FILTER_HPP
#define FILTER_HPP

#include <vector>
#include <map>

#include "graphTypes.hpp"

void computeVerticesAndClusterQuality(std::vector<Cluster>& clusters);
void computeClusterDeviation(std::vector<Cluster>& clusters);
void computeClusterTresholds(std::vector<Cluster>& clusters, double deviation_factor);
void filterVertices(std::vector<Cluster>& clusters);

#endif // FILTER_HPP
