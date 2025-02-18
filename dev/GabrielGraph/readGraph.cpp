#include "readGraph.hpp"

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <variant>
#include <iostream>
#include <cctype>
#include <algorithm>

#include "stringHelpers.hpp"

using namespace std;

int readGraph(ClusterMap& clusters, const std::string& input_filename_with_path)
{
  ifstream file(input_filename_with_path);

  if (!file.is_open()) {
    cerr << "Failed to open file: " << input_filename_with_path << endl;
    return -1;
  }

  string line;
  size_t lineNo = 0;
  bool useUnsigned = false;
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
        useUnsigned = true;
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

    if (useUnsigned) {
      unsigned classID = 0;

      try {
        classID = stoul(classToken);
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
