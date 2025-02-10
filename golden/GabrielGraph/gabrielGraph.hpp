#ifndef GABRIEL_GRAPH_HPP
#define GABRIEL_GRAPH_HPP

#include <cmath>
#include <vector>

#include "squaredDistance.hpp"

void gabrielGraph(std::vector<std::pair<size_t, size_t>>& edges, std::vector<std::vector<double>> points);
void computeMidpoint(std::vector<double>& midpoint, const std::vector<double>& a, const std::vector<double>& b, const size_t& num_coords);
void checkGabrielEdge(bool& isGabrielEdge, const std::vector<std::vector<double>>& points, const std::vector<double>& midpoint, const double& radius_sq,
                      const size_t& num_points, const size_t& vertex1, const size_t& vertex2);

#endif // GABRIEL_GRAPH_HPP