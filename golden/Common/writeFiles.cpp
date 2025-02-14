#include "writeFiles.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <variant>
#include <string>
#include <type_traits>
#include <algorithm>

#include "graphTypes.hpp"
#include "stringHelpers.hpp"

using namespace std;

/*

  output format:

  vertex0_id, |, feature0_0, feature0_1, ..., feature0_n, |, cluster0_id, |, adjacent_vertex0_0, adjacent_vertex0_1, ..., adjacent_vertex0_m
  vertex1_id, |, feature1_0, feature1_1, ..., feature1_n, |, cluster1_id, |, adjacent_vertex1_0, adjacent_vertex1_1, ..., adjacent_vertex1_m
  ...
  vertexk_id, |, featurek_0, featurek_1, ..., featurek_n, |, clusterk_id, |, adjacent_vertexk_0, adjacent_vertexk_1, ..., adjacent_vertexk_m

*/

int writeGraphToFile(const ClusterMap& clusters, const std::string& file_name_with_path)
{
  ofstream outfile(file_name_with_path);

  if (!outfile.is_open()) {
    cerr << "Error: Unable to open file " << file_name_with_path << " for writing." << endl;
    return -1;
  }
  
  // Iterate over each cluster.
  for (const auto& clusterPair : clusters) {
    const ClassType& clusterKey = clusterPair.first;
    const Cluster& cluster = clusterPair.second;
    
    // Iterate over each vertex in the cluster.
    for (const Vertex& vertex : cluster.vertices) {
      // Write vertex id.
      outfile << vertex.id << ", |, ";
      
      // Write features separated by comma.
      bool firstFeature = true;
      for (const double& f : vertex.features) {
        if (!firstFeature) {
          outfile << ", ";
        }

        outfile << f;
        firstFeature = false;
      }
      
      // Write cluster id.
      outfile << ", |, " << clusterKey << ", |, ";
      
      // Write adjacent vertices.
      bool firstAdjacent = true;
      for (const auto& adj : vertex.adjacents) {
        if (!firstAdjacent) {
          outfile << ", ";
        }
        
        outfile << adj->id;
        firstAdjacent = false;
      }
      
      outfile << "\n";
    }
  }
  
  outfile.close();
  return 0;
}
