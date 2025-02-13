#ifndef WRITEFILES_H
#define WRITEFILES_H

#include <vector>
#include <map>
#include <string>

#include "graphTypes.hpp"

/*

  output format:

  output format:

  vertex0_id, |, feature0_0, feature0_1, ..., feature0_n, |, cluster0_id, |, adjacent_vertex0_0, adjacent_vertex0_1, ..., adjacent_vertex0_m
  vertex1_id, |, feature1_0, feature1_1, ..., feature1_n, |, cluster1_id, |, adjacent_vertex1_0, adjacent_vertex1_1, ..., adjacent_vertex1_m
  ...
  vertexk_id, |, featurek_0, featurek_1, ..., featurek_n, |, clusterk_id, |, adjacent_vertexk_0, adjacent_vertexk_1, ..., adjacent_vertexk_m

*/

int writeGraphToFile(const ClusterMap& clusters, const std::string& file_name_with_path);

#endif // WRITEFILES_H
