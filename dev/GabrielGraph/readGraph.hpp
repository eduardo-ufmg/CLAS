#ifndef READVERTICES_HPP
#define READVERTICES_HPP

#include <vector>
#include <string>
#include <variant>

#include "graphTypes.hpp"

int readGraph(ClusterMap& clusters, const std::string& input_filename_with_path);

#endif // READVERTICES_HPP
