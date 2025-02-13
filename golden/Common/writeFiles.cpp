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
    
    // Convert the cluster key (ClassType) to a string.
    string clusterKeyStr;

    std::visit([&clusterKeyStr](auto&& arg) {

      if constexpr(std::is_same_v<std::decay_t<decltype(arg)>, unsigned>) {
        clusterKeyStr = to_string(arg);
      } else {
        clusterKeyStr = arg;
      }
      
    }, clusterKey);
    
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
      outfile << ", |, " << clusterKeyStr << ", |, ";
      
      // Write adjacent vertices.
      bool firstAdjacent = true;
      for (const auto& adj : vertex.adjacents) {
        if (!firstAdjacent) {
          outfile << ", ";
        }
        
        outfile << adj;
        firstAdjacent = false;
      }
      
      outfile << "\n";
    }
  }
  
  outfile.close();
  return 0;
}
