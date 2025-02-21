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

VertexID_t parseVertexID(const string& tokens);
vector<double> parseFeatures(const string& tokens);
ClassType parseClusterID(const string& tokens);
AdjacencyVector parseAdjacents(const string& tokens);

pair<size_t, size_t> addVertexToCluster(ClusterMap& clusters, const VertexID_t& vertexId, const vector<double>& features, const ClassType& clusterKey, const AdjacencyVector& adjacents);

unsigned parseExpertid(const string& tokens);
Edge parseEdge(const string& tokens);
vector<double> parseExpertDifferences(const string& tokens);
vector<double> parseMidpointCoordinates(const string& tokens);
double parseBias(const string& tokens);

/*

  input format:

  vertex0_id, |, feature0_0, feature0_1, ..., feature0_n, |, cluster0_id, |, adjacent_vertex0_0 - isSupportEdge, adjacent_vertex0_1 - isSupportEdge, ..., adjacent_vertex0_m - isSupportEdge
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
    
    VertexID_t vertexId = parseVertexID(parts[0]);

    vector<double> features = parseFeatures(parts[1]);
    
    ClassType clusterKey = parseClusterID(parts[2]);
    
    AdjacencyVector adjacents = parseAdjacents(parts[3]);
    
    addVertexToCluster(clusters, vertexId, features, clusterKey, adjacents);
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

    ClassType clusterid = parseClusterID(tokens.back());

    vector<double> features = parseFeatures(tokens.front());

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

int readExperts(vector<Expert>& experts, const string& input_filename_with_path)
{
  ifstream infile = openFileRead(input_filename_with_path);

  string line;

  while (getline(infile, line)) {
    Expert expert;
    
    vector<string> parts = split(line, ", |,", 5);

    expert.id = parseExpertid(parts[0]);

    expert.edge = parseEdge(parts[1]);

    expert.differences = parseExpertDifferences(parts[2]);

    expert.midpoint_coordinates = parseMidpointCoordinates(parts[3]);

    expert.bias = parseBias(parts[4]);

    experts.push_back(expert);
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

    vertices.emplace(-lineNo, make_shared<Vertex>(Vertex(parseFeatures(line), {}, nullptr)));

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

VertexID_t parseVertexID(const string& tokens)
{
  string vertexIdStr = trim(tokens);

  VertexID_t vertexId;

  try {
    vertexId = stoi(vertexIdStr);
  } catch (const exception& e) {
    cerr << "Error: could not convert vertex ID '" << vertexIdStr << "' to integer." << endl;
    exit(1);
  }

  return vertexId;
}

vector<double> parseFeatures(const string& tokens)
{
  vector<string> featureTokens = split(tokens, ',');

  vector<double> features;

  for (const string& token : featureTokens) {
    string trimmed = trim(token);

    if (trimmed.empty()) {
      trimmed = "0.0";
    }

    try {
      features.push_back(stod(trimmed));
    } catch (const exception& e) {
      cerr << "Error: could not convert feature token '" << trimmed << "' to double." << endl;
      exit(1);
    }
  }

  return features;
}

ClassType parseClusterID(const string& tokens)
{
  string clusteridstr = trim(tokens);

  ClassType clusterid;

  if (clusteridstr.empty()) {
    clusterid = "unclassified";
    return clusterid;
  } 

  if (clusteridstr[0] == '-' ?
        all_of(clusteridstr.begin() + 1, clusteridstr.end(), ::isdigit) :
        all_of(clusteridstr.begin(), clusteridstr.end(), ::isdigit)) {
    try {
      clusterid = stoi(clusteridstr);
    } catch (const exception& e) {
      cerr << "Error: could not convert cluster ID '" << clusteridstr << "' to integer." << endl;
      exit(1);
    }
  } else {
    clusterid = clusteridstr;
  }

  return clusterid;
}

AdjacencyVector parseAdjacents(const string& tokens)
{
  vector<string> adjacentsTokens = split(tokens, ',');

  AdjacencyVector adjacents;

  for (const string& token : adjacentsTokens) {
    string trimmed = trim(token);

    if (trimmed.empty()) {
      continue;
    }

    vector<string> adjacentParts = split(trimmed, "-");

    if (adjacentParts.size() != 2) {
      cerr << "Error: expected 2 parts in adjacent token '" << trimmed << "', but found " << adjacentParts.size() << " parts." << endl;
      exit(1);
    }

    string vertexIdStr = trim(adjacentParts[0]);
    string isSupportEdgeStr = trim(adjacentParts[1]);

    VertexID_t vertexid;

    try {
      vertexid = stoi(vertexIdStr);
    } catch (const exception& e) {
      cerr << "Error: could not convert vertex ID '" << vertexIdStr << "' to integer." << endl;
      exit(1);
    }

    bool isSupportEdge;

    try {
      isSupportEdge = static_cast<bool>(stoi(isSupportEdgeStr));
    } catch (const exception& e) {
      cerr << "Error: could not convert isSupportEdge token '" << isSupportEdgeStr << "' to boolean." << endl;
      exit(1);
    }

    adjacents.push_back(make_pair(vertexid, isSupportEdge));
  }

  return adjacents;
}

pair<size_t, size_t> addVertexToCluster(ClusterMap& clusters, const VertexID_t& vertexId, const vector<double>& features, const ClassType& clusterKey, const AdjacencyVector& adjacents)
{
  clusters.emplace(clusterKey, Cluster());
  clusters[clusterKey].addVertex(vertexId, make_shared<Vertex>(Vertex(features, adjacents, &clusters[clusterKey])));
  return make_pair(clusters[clusterKey].vertices.size(), clusters.size());
}

unsigned parseExpertid(const string& tokens)
{
  string expertidStr = trim(tokens);

  unsigned expertid;

  try {
    expertid = stoi(expertidStr);
  } catch (const exception& e) {
    cerr << "Error: could not convert expert ID '" << expertidStr << "' to integer." << endl;
    exit(1);
  }

  return expertid;
}
Edge parseEdge(const string& tokens)
{
  vector<string> edgeParts = split(tokens, ',');

  if (edgeParts.size() != 2) {
    cerr << "Error: expected 2 parts in edge token '" << tokens << "', but found " << edgeParts.size() << " parts." << endl;
    exit(1);
  }

  VertexID_t vertex0, vertex1;

  try {
    vertex0 = stoi(trim(edgeParts[0]));
    vertex1 = stoi(trim(edgeParts[1]));
  } catch (const exception& e) {
    cerr << "Error: could not convert edge token '" << tokens << "' to integers." << endl;
    exit(1);
  }

  return make_pair(vertex0, vertex1);
}

vector<double> parseExpertDifferences(const string& tokens)
{
  vector<string> differencesTokens = split(tokens, ',');

  vector<double> differences;

  for (const string& token : differencesTokens) {
    string trimmed = trim(token);

    if (trimmed.empty()) {
      trimmed = "0.0";
    }

    try {
      differences.push_back(stod(trimmed));
    } catch (const exception& e) {
      cerr << "Error: could not convert differences token '" << trimmed << "' to double." << endl;
      exit(1);
    }
  }

  return differences;
}

vector<double> parseMidpointCoordinates(const string& tokens)
{
  vector<string> midpointTokens = split(tokens, ',');

  vector<double> midpointCoordinates;

  for (const string& token : midpointTokens) {
    string trimmed = trim(token);

    if (trimmed.empty()) {
      trimmed = "0.0";
    }

    try {
      midpointCoordinates.push_back(stod(trimmed));
    } catch (const exception& e) {
      cerr << "Error: could not convert midpoint token '" << trimmed << "' to double." << endl;
      exit(1);
    }
  }

  return midpointCoordinates;
}

double parseBias(const string& tokens)
{
  string biasStr = trim(tokens);

  double bias;

  try {
    bias = stod(biasStr);
  } catch (const exception& e) {
    cerr << "Error: could not convert bias token '" << biasStr << "' to double." << endl;
    exit(1);
  }

  return bias;
}
