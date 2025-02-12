#include "writeFiles.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "graphTypes.hpp"

using namespace std;

/*

  output format:

  vertex0_id, |, feature0_0, feature0_1, ..., feature0_n, |, cluster0_id, |, adjacent_vertex0_0, adjacent_vertex0_1, ..., adjacent_vertex0_m
  vertex1_id, |, feature1_0, feature1_1, ..., feature1_n, |, cluster1_id, |, adjacent_vertex1_0, adjacent_vertex1_1, ..., adjacent_vertex1_m
  ...
  vertexk_id, |, featurek_0, featurek_1, ..., featurek_n, |, clusterk_id, |, adjacent_vertexk_0, adjacent_vertexk_1, ..., adjacent_vertexk_m

*/

int writeGraphToFile(const std::vector<Cluster>& clusters, const std::string& file_name_with_path)
{
  ofstream file(file_name_with_path.c_str());
  if (!file.is_open()) {
    cerr << "Error: Could not open file " << file_name_with_path << " for writing." << endl;
    return 1;
  }

  for (auto cluster : clusters) {
    for (auto vertex : cluster.vertices) {
      file << vertex.id;
      file << ",\t|";
      for (auto feature : vertex.features) {
        file << ", " << feature;
      }
      file << ",\t|";
      file << ", " << cluster.id;
      file << ",\t|";
      for (auto adjacent_vertex : vertex.adjacents) {
        file << ", " << adjacent_vertex;
      }
      file << endl;
    }
  }

  file.close();

  return 0;
}
