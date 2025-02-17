#include "readGraph.hpp"

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <unordered_map>

#include "graphTypes.hpp"
#include "stringHelpers.hpp"

using namespace std;

/*

  input format:

  vertex0_id, |, feature0_0, feature0_1, ..., feature0_n, |, cluster0_id, |, adjacent_vertex0_0 - isSupportEdge, adjacent_vertex0_1 - isSupportEdge, ..., adjacent_vertex0_m - isSupportEdge
  vertex1_id, |, feature1_0, feature1_1, ..., feature1_n, |, cluster1_id, |, adjacent_vertex1_0 - isSupportEdge, adjacent_vertex1_1 - isSupportEdge, ..., adjacent_vertex1_m - isSupportEdge
  ...
  vertexk_id, |, featurek_0, featurek_1, ..., featurek_n, |, clusterk_id, |, adjacent_vertexk_0 - isSupportEdge, adjacent_vertexk_1 - isSupportEdge, ..., adjacent_vertexk_m - isSupportEdge

*/

int readGraph(ClusterMap& clusters, const std::string& input_file_name_with_path)
{
  ifstream input_file(input_file_name_with_path.c_str());

  if (!input_file.is_open()) {
    cerr << "Error: could not open file " << input_file_name_with_path << endl;
    return 1;
  }
  
  string line;
  // Process each line of the file.
  while(getline(input_file, line)) {
    // Skip empty lines.
    if (line.empty()) {
      continue;
    }
    
    // Split the line using the literal delimiter ", |, "
    // Expected parts:
    //   0: vertex id
    //   1: features (comma-separated list)
    //   2: cluster id
    //   3: adjacent vertices (comma-separated list)
    vector<string> parts = split(line, ", |, ");

    if (parts.size() != 4) {
      cerr << "Warning: unexpected format in line: " << line << endl;
      continue;
    }
    
    // --- Parse Vertex ID ---
    string vertexIdStr = trim(parts[0]);
    VertexID_t vertexId;

    try {
      vertexId = static_cast<VertexID_t>(stoul(vertexIdStr));
    } catch (const std::exception& e) {
      cerr << "Error converting vertex id '" << vertexIdStr << "' to unsigned: " << e.what() << endl;
      return 1;
    }
    
    // --- Parse Features ---
    string featuresStr = trim(parts[1]);
    vector<string> featureTokens = split(featuresStr, ",");
    vector<double> features;

    for (const auto& token : featureTokens) {
      string trimmedToken = trim(token);

      if (!trimmedToken.empty()) {
        try {
          features.push_back(stod(trimmedToken));
        } catch (const std::exception& e) {
          cerr << "Error converting feature '" << trimmedToken << "' to double in line: " << line << endl;
          return 1;
        }
      }

    }
    
    // --- Parse Cluster ID ---
    string clusterIdStr = trim(parts[2]);
    // Determine whether to treat the cluster id as an unsigned or string.
    bool isClusterIdUnsigned = !clusterIdStr.empty() && all_of(clusterIdStr.begin(), clusterIdStr.end(), ::isdigit);

    ClassType clusterKey;

    if (isClusterIdUnsigned) {
      try {
        clusterKey = static_cast<unsigned>(stoul(clusterIdStr));
      } catch (const std::exception& e) {
        cerr << "Error converting cluster id '" << clusterIdStr << "' to unsigned in line: " << line << endl;
        return 1;
      }
    } else {
      clusterKey = clusterIdStr;
    }
    
    // --- Parse Adjacent Vertices ---
    string adjacentsStr = trim(parts[3]);
    vector<string> adjacentTokens = split(adjacentsStr, ",");
    vector< pair<VertexID_t, bool> > adjacents;

    for (const auto& token : adjacentTokens) {
      vector<string> adjParts = split(token, " - ");

      if (!adjParts.empty()) {
        try {
          adjacents.push_back({static_cast<VertexID_t>(stoul(trim(adjParts[0]))), static_cast<bool>(stoi(trim(adjParts[1])))});
        } catch (const std::exception& e) {
          cerr << "Error converting adjacent vertex id '" << trim(token) << "' to unsigned in line: " << line << endl;
          return 1;
        }
      }

    }
    
    // --- Create and Insert Vertex ---

    // Insert the vertex into the appropriate cluster.
    clusters.emplace(clusterKey, Cluster());

    clusters[clusterKey].vertices.emplace(vertexId, make_shared<Vertex>(Vertex(features, &clusters[clusterKey])));

    clusters[clusterKey].vertices[vertexId]->adjacents = adjacents;

  }
  
  input_file.close();

  return 0;
}
