#ifndef readGraph_HPP
#define readGraph_HPP

#include <vector>
#include <string>

#include "graphTypes.hpp"

int readGraph(ClusterMap& clusters, const std::string& input_filename_with_path);
int readVertices(ClusterMap& clusters, const std::string& input_filename_with_path);
int readExperts(std::vector<Expert>& experts, const std::string& input_filename_with_path);
int readVertices(vector<Vertex>& vertices, const std::string& input_filename_with_path);

#endif // readGraph_HPP
