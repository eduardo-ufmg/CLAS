#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>
#include <variant>

#include "graphTypes.hpp"
#include "stringHelpers.hpp"
#include "readGraph.hpp"
#include "gabrielGraph.hpp"
#include "writeFiles.hpp"

using namespace std;

int main(int argc, char* argv[])
{
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " <input_csv>" << endl;
    return 1;
  }

  string input_filename_with_path = argv[1];

  ClusterMap clusters;

  if (readGraph(clusters, input_filename_with_path) != 0) {
    return 1;
  }

  cout << "Read " << clusters.size() << " clusters." << endl;
  
  for (const auto& clusterPair : clusters) {
    const ClassType& clusterKey = clusterPair.first;
    const Cluster& cluster = clusterPair.second;
    
    string clusterKeyStr;

    std::visit([&clusterKeyStr](auto&& arg) {

      if constexpr(std::is_same_v<std::decay_t<decltype(arg)>, unsigned>) {
        clusterKeyStr = to_string(arg);
      } else {
        clusterKeyStr = arg;
      }
      
    }, clusterKey);
    
    cout << "Cluster " << clusterKeyStr << " has " << cluster.vertices.size() << " vertices." << endl;
  }

  gabrielGraph(clusters);

  cout << "Some adjacency lists:" << endl;

  int clusterCount = 0;
  for (const auto& clusterPair : clusters) {
    if (clusterCount >= 4) break;
    
    const Cluster& cluster = clusterPair.second;
    int vertexCount = 0;
    
    for (const auto& vertex : cluster.vertices) {
      if (vertexCount >= 4) break;
      
      cout << "Vertex " << vertex.id << ": ";
      
      for (const auto& neighbor : vertex.adjacents) {
        cout << neighbor << " ";
      }
      cout << endl;
      
      vertexCount++;
    }
    
    clusterCount++;
  }

  string output_filename_with_path = outputPathFromInputPath(input_filename_with_path);

  if (writeGraphToFile(clusters, output_filename_with_path) != 0) {
    return 1;
  }

  return 0;
}
