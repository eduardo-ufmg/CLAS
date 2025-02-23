#ifndef readGraph_HPP
#define readGraph_HPP

#include <vector>
#include <string>

#include "graphTypes.hpp"

int readGraph(ClusterMap& clusters, const std::string& input_filename_with_path);
int readVertices(ClusterMap& clusters, const std::string& input_filename_with_path);
int readExperts(Experts& experts, const std::string& input_filename_with_path);
int readVertices(VertexMap& vertices, const std::string& input_filename_with_path);

#endif // readGraph_HPP
