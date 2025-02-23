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

ifstream openFileRead(const string& input_filename_with_path);

VertexID parseVertexID(const string& token);
const Coordinates parseCoordinates(const string& tokens);
ClusterID parseClusterID(const string& token);
const AdjacencyVector parseAdjacents(const string& tokens);

pair<size_t, size_t> addVertexToCluster(ClusterMap& clusters, const VertexID vertexid, const Coordinates features, const ClusterID clusterKey, const AdjacencyVector adjacents);

ExpertID parseExpertid(const string& token);
const Edge parseEdge(const string& token);
const ExpertDifferences parseExpertDifferences(const string& tokens);
const Coordinates parseMidpointCoordinates(const string& tokens);
double parseBias(const string& token);

/*

  input format:

  vertex0_id, |, feature0_0, feature0_1, ..., feature0_n, |, cluster0_id, |, adjacent_vertex0_0 - isSE, adjacent_vertex0_1 - isSupportEdge, ..., adjacent_vertex0_m - isSupportEdge
  vertex1_id, |, feature1_0, feature1_1, ..., feature1_n, |, cluster1_id, |, adjacent_vertex1_0 - isSupportEdge, adjacent_vertex1_1 - isSupportEdge, ..., adjacent_vertex1_m - isSupportEdge
  ...
  vertexk_id, |, featurek_0, featurek_1, ..., featurek_n, |, clusterk_id, |, adjacent_vertexk_0 - isSupportEdge, adjacent_vertexk_1 - isSupportEdge, ..., adjacent_vertexk_m - isSupportEdge

*/

int readGraph(ClusterMap& clusters, const string& input_filename_with_path)
{
  ifstream infile = openFileRead(input_filename_with_path);
  
  string line;

  while(getline(infile, line)) {
    // Skip empty lines.
    if (line.empty()) {
      continue;
    }
    
    vector<string> parts = split(line, ", |,", 4);
    
    VertexID vertexid = parseVertexID(parts[0]);

    const Coordinates features = parseCoordinates(parts[1]);
    
    ClusterID clusterKey = parseClusterID(parts[2]);
    
    const AdjacencyVector adjacents = parseAdjacents(parts[3]);
    
    addVertexToCluster(clusters, vertexid, features, clusterKey, adjacents);
  }
  
  infile.close();

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
  ifstream infile = openFileRead(input_filename_with_path);

  string line;
  size_t lineNo = 0;

  while (getline(infile, line)) {

    if (line.empty()) {
      continue;
    }

    vector<string> tokens = split(line, ',', DIRECTION::BACKWARD, 1);

    ClusterID clusterid = parseClusterID(tokens.back());

    const Coordinates features = parseCoordinates(tokens.front());

    addVertexToCluster(clusters, lineNo, features, clusterid, {});

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

int readExperts(Experts& experts, const string& input_filename_with_path)
{
  ifstream infile = openFileRead(input_filename_with_path);

  string line;

  while (getline(infile, line)) {
    vector<string> parts = split(line, ", |,", 5);

    ExpertID id = parseExpertid(parts[0]);

    const Edge edge = parseEdge(parts[1]);

    const ExpertDifferences differences = parseExpertDifferences(parts[2]);

    const Coordinates mpCoordinates = parseMidpointCoordinates(parts[3]);

    double bias = parseBias(parts[4]);

    experts.emplace_back(edge, differences, mpCoordinates, bias, id);
  }

  infile.close();
  return 0;
}

/*

  input format:

  feature0_0, feature0_1, ..., feature0_n
  feature1_0, feature1_1, ..., feature1_n
  ...
  featurek_0, featurek_1, ..., featurek_n

*/
int readVertices(VertexMap& vertices, const string& input_filename_with_path)
{
  ifstream infile = openFileRead(input_filename_with_path);

  string line;
  size_t lineNo = 0;

  while (getline(infile, line)) {

    if (line.empty()) {
      continue;
    }

    const Coordinates features = parseCoordinates(line);

    vertices.emplace(-(lineNo + 1), Vertex(features, {}, nullptr));

    ++lineNo;
  }

  return 0;
}

ifstream openFileRead(const string& input_filename_with_path)
{
  ifstream infile(input_filename_with_path);

  if (!infile.is_open()) {
    cerr << "Error: could not open infile " << input_filename_with_path << endl;
    exit(1);
  }

  return infile;
}

VertexID parseVertexID(const string& token)
{
  string vertexidstr = trim(token);

  VertexID vertexid;

  try {
    vertexid = stoi(vertexidstr);
  } catch (exception& e) {
    cerr << "Error: could not convert vertex ID '" << vertexidstr << "' to integer." << endl;
    exit(1);
  }

  return vertexid;
}

const Coordinates parseCoordinates(const string& tokens)
{
  vector<string> featureTokens = split(tokens, ',');

  Coordinates features;

  for (string token : featureTokens) {
    string trimmed = trim(token);

    if (trimmed.empty()) {
      trimmed = "0.0";
    }

    try {
      features.push_back(stod(trimmed));
    } catch (exception& e) {
      cerr << "Error: could not convert feature token '" << trimmed << "' to double." << endl;
      exit(1);
    }
  }

  return features;
}

ClusterID parseClusterID(const string& token)
{
  string clusteridstr = trim(token);

  ClusterID clusterid;

  if (clusteridstr.empty()) {
    clusterid = "unlabeled";
    return clusterid;
  } 

  if (clusteridstr[0] == '-' ?
        all_of(clusteridstr.begin() + 1, clusteridstr.end(), ::isdigit) :
        all_of(clusteridstr.begin(), clusteridstr.end(), ::isdigit)) {
    try {
      clusterid = stoi(clusteridstr);
    } catch (exception& e) {
      cerr << "Error: could not convert cluster ID '" << clusteridstr << "' to integer." << endl;
      exit(1);
    }
  } else {
    clusterid = clusteridstr;
  }

  return clusterid;
}

const AdjacencyVector parseAdjacents(const string& tokens)
{
  vector<string> adjacentsTokens = split(tokens, ',');

  AdjacencyVector adjacents;

  for (string token : adjacentsTokens) {
    string trimmed = trim(token);

    if (trimmed.empty()) {
      continue;
    }

    vector<string> adjacentParts = split(trimmed, "-", 2);

    string vertexidstr = trim(adjacentParts[0]);
    string isSEstr = trim(adjacentParts[1]);

    VertexID vertexid;

    try {
      vertexid = stoi(vertexidstr);
    } catch (exception& e) {
      cerr << "Error: could not convert vertex ID '" << vertexidstr << "' to integer." << endl;
      exit(1);
    }

    bool isSE;

    try {
      isSE = static_cast<bool>(stoi(isSEstr));
    } catch (exception& e) {
      cerr << "Error: could not convert isSE token '" << isSEstr << "' to boolean." << endl;
      exit(1);
    }

    adjacents.push_back(make_pair(vertexid, isSE));
  }

  return adjacents;
}

pair<size_t, size_t> addVertexToCluster(ClusterMap& clusters, const VertexID vertexid, const Coordinates features, const ClusterID clusterKey, const AdjacencyVector adjacents)
{
  clusters.emplace(clusterKey, Cluster());
  clusters[clusterKey].addVertex(vertexid, Vertex(features, adjacents, &clusters[clusterKey]));
  return make_pair(clusters[clusterKey].vertices.size(), clusters.size());
}

ExpertID parseExpertid(const string& token)
{
  string expertidStr = trim(token);

  ExpertID expertid;

  try {
    expertid = stoi(expertidStr);
  } catch (exception& e) {
    cerr << "Error: could not convert expert ID '" << expertidStr << "' to integer." << endl;
    exit(1);
  }

  return expertid;
}
const Edge parseEdge(const string& token)
{
  vector<string> edgeParts = split(token, ",", 2);

  VertexID vertex0, vertex1;

  try {
    vertex0 = stoi(trim(edgeParts[0]));
    vertex1 = stoi(trim(edgeParts[1]));
  } catch (exception& e) {
    cerr << "Error: could not convert edge token '" << token << "' to integers." << endl;
    exit(1);
  }

  return make_pair(vertex0, vertex1);
}

const ExpertDifferences parseExpertDifferences(const string& tokens)
{
  vector<string> differencesTokens = split(tokens, ',');

  ExpertDifferences differences;

  for (string token : differencesTokens) {
    string trimmed = trim(token);

    if (trimmed.empty()) {
      trimmed = "0.0";
    }

    try {
      differences.push_back(stod(trimmed));
    } catch (exception& e) {
      cerr << "Error: could not convert differences token '" << trimmed << "' to double." << endl;
      exit(1);
    }
  }

  return differences;
}

const Coordinates parseMidpointCoordinates(const string& tokens)
{
  vector<string> midpointTokens = split(tokens, ',');

  Coordinates midpointCoordinates;

  for (string token : midpointTokens) {
    string trimmed = trim(token);

    if (trimmed.empty()) {
      trimmed = "0.0";
    }

    try {
      midpointCoordinates.push_back(stod(trimmed));
    } catch (exception& e) {
      cerr << "Error: could not convert midpoint token '" << trimmed << "' to double." << endl;
      exit(1);
    }
  }

  return midpointCoordinates;
}

double parseBias(const string& token)
{
  string biasstr = trim(token);

  double bias;

  try {
    bias = stod(biasstr);
  } catch (exception& e) {
    cerr << "Error: could not convert bias token '" << biasstr << "' to double." << endl;
    exit(1);
  }

  return bias;
}
