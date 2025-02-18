#include "readFiles.hpp"

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

int readGraph(ClusterMap& clusters, const string& input_filename_with_path)
{
  ifstream input_file(input_filename_with_path.c_str());

  if (!input_file.is_open()) {
    cerr << "Error: could not open file " << input_filename_with_path << endl;
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
    bool isClusterIdInt = !clusterIdStr.empty() && all_of(clusterIdStr.begin(), clusterIdStr.end(), ::isdigit);

    ClassType clusterKey;

    if (isClusterIdInt) {
      try {
        clusterKey = static_cast<int>(stoi(clusterIdStr));
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

/*

  input format:

  feature0_0, feature0_1, ..., feature0_n, cluster0_id
  feature1_0, feature1_1, ..., feature1_n, cluster1_id
  ...
  featurek_0, featurek_1, ..., featurek_n, clusterk_id

*/

int readVertices(ClusterMap& clusters, const string& input_filename_with_path)
{
  ifstream file(input_filename_with_path);

  if (!file.is_open()) {
    cerr << "Failed to open file: " << input_filename_with_path << endl;
    return -1;
  }

  string line;
  size_t lineNo = 0;
  bool useInt = false;
  bool firstLine = true;

  while (getline(file, line)) {

    if (line.empty()) {
      continue;
    }

    vector<string> tokens = split(line, ',');

    if (tokens.size() < 2) {
      cerr << "Line " << lineNo << " has insufficient tokens." << endl;
      continue;
    }

    // The last token is the class ID.
    string classToken = trim(tokens.back());
    tokens.pop_back(); // Remaining tokens are the feature coordinates.

    // On the first valid line, decide if the classID should be an unsigned integer.
    if (firstLine) {
      // Check if the classToken is composed entirely of digits.
      if (!classToken.empty() && all_of(classToken.begin(), classToken.end(), ::isdigit)) {
        useInt = true;
      }

      firstLine = false;
    }

    // Convert the coordinate tokens into a vector of doubles.
    vector<double> features;

    for (auto& token : tokens) {
      string trimmed = trim(token);

      if (trimmed.empty()) {
        trimmed = "0.0";
      }

      try {
        features.push_back(stod(trimmed));
      } catch (const exception& e) {
        cerr << "Error converting '" << trimmed << "' to double on line " << lineNo << endl;
        return -1;
      }

    }

    // Insert the vertex into the corresponding Cluster based on the class ID.
    ClassType key;

    if (useInt) {
      int classID = 0;

      try {
        classID = stoi(classToken);
      } catch (const exception& e) {
        cerr << "Error converting class token '" << classToken << "' to unsigned on line " << lineNo << endl;
        return -1;
      }

      key = classID;

    } else {

      key = classToken;

    }

    clusters.emplace(key, Cluster());

    VertexID_t vertexID = lineNo;

    clusters[key].vertices.emplace(vertexID, make_shared<Vertex>(Vertex(features, &clusters[key])));

    ++lineNo;
  }

  return 0;
}

/*

  input format:

  id0, |, v00, v01, |, diff_coord00, diff_coord01, ..., diff_coord0n, |, midpoint_coord00, midpoint_coord01, ..., midpoint_coord0n, |, bias0
  id1, |, v10, v11, |, diff_coord10, diff_coord11, ..., diff_coord1n, |, midpoint_coord10, midpoint_coord11, ..., midpoint_coord1n, |, bias1
  ...
  idk, |, vk0, vk1, |, diff_coordk0, diff_coordk1, ..., diff_coordkn, |, midpoint_coordk0, midpoint_coordk1, ..., midpoint_coordkn, |, biask

*/

int readExperts(vector<Expert>& experts, const string& input_filename_with_path)
{
  ifstream input_file(input_filename_with_path);
  if (!input_file.is_open()) {
    cerr << "Error: could not open file " << input_filename_with_path << endl;
    return 1;
  }

  string line;
  while (getline(input_file, line)) {
    Expert expert;
    istringstream iss(line);
    
    vector<string> parts = split(line, ", |,");

    expert.id = stoul(trim(parts[0]));

    vector<string> edge_tokens = split(parts[1], ",");

    expert.edge = Edge(static_cast<VertexID_t>(stoul(trim(edge_tokens[0]))), static_cast<VertexID_t>(stoul(trim(edge_tokens[1]))));

    vector<string> differences_tokens = split(parts[2], ",");

    for (const string& token : differences_tokens) {
      expert.differences.push_back(stod(trim(token)));
    }

    vector<string> midpoint_coordinates_tokens = split(parts[3], ",");

    for (const string& token : midpoint_coordinates_tokens) {
      expert.midpoint_coordinates.push_back(stod(trim(token)));
    }

    expert.bias = stod(trim(parts[4]));

    experts.push_back(expert);
  }

  input_file.close();
  return 0;
}

/*

  input format:

  feature0_0, feature0_1, ..., feature0_n
  feature1_0, feature1_1, ..., feature1_n
  ...
  featurek_0, featurek_1, ..., featurek_n

*/
int readVertices(vector<Vertex>& vertices, const string& input_filename_with_path)
{
  ifstream file(input_filename_with_path);

  if (!file.is_open()) {
    cerr << "Failed to open file: " << input_filename_with_path << endl;
    return -1;
  }

  string line;
  size_t lineNo = 0;

  while (getline(file, line)) {

    if (line.empty()) {
      continue;
    }

    vector<string> tokens = split(line, ',');

    // Convert the coordinate tokens into a vector of doubles.
    vector<double> features;

    for (auto& token : tokens) {
      string trimmed = trim(token);

      if (trimmed.empty()) {
        trimmed = "0.0";
      }

      try {
        features.push_back(stod(trimmed));
      } catch (const exception& e) {
        cerr << "Error converting '" << trimmed << "' to double on line " << lineNo << endl;
        return -1;
      }

    }

    Vertex vertex;

    vertex.features = features;

    vertices.push_back(vertex);

    ++lineNo;
  }

  return 0;
}
