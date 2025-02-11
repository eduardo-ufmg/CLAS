#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cstdlib>

#include "readFiles.hpp"
#include "stringHelpers.hpp"
#include "filter.hpp"
#include "writeFiles.hpp"

using namespace std;

int main(int argc, char* argv[])
{
  if(argc < 4) {
    cerr << "Usage: " << argv[0] << " <features>.csv <targets>.csv <edges>.csv" << endl;
    return 1;
  }

  string features_file_name_with_path = argv[1];
  string targets_file_name_with_path = argv[2];
  string edges_file_name_with_path = argv[3];

  // -------------------------------
  // 1. Read vertices.csv
  // -------------------------------
  vector< vector<double> > vertices;
  if (readVertices(vertices, features_file_name_with_path) != 0) {
    return 1;
  }

  size_t numVertices = vertices.size();
  if (numVertices == 0) {
    cerr << "Error: No vertices were read." << endl;
    return 1;
  }

  // -------------------------------
  // 2. Read targets.csv
  // -------------------------------
  vector<int> targets;
  if (readTargets(targets, targets_file_name_with_path) != 0) {
    return 1;
  }

  if(targets.size() != numVertices) {
    cerr << "Error: Number of targets (" << targets.size() 
      << ") does not match number of vertices (" << numVertices << ")." << endl;
    return 1;
  }

  // -------------------------------
  // 3. Read edges.csv
  // -------------------------------
  vector< pair<int,int> > edges;
  if (readEdges(edges, edges_file_name_with_path) != 0) {
    return 1;
  }

  // -------------------------------
  // 4. Build the graph (adjacency list)
  // -------------------------------
  vector< vector<int> > adj(numVertices);
  buildGraph(adj, edges, numVertices);

  // -------------------------------
  // 5. Compute q(x_i) for each vertex
  // -------------------------------
  vector<double> q(numVertices, 0.0);
  computeQ(q, adj, targets, numVertices);

  // -------------------------------
  // 6. Compute the per-class threshold t_class
  // -------------------------------
  map<int, pair<double, int>> classStats;
  computeClassStats(classStats, targets, q, numVertices);

  map<int, double> threshold;
  computeThresholds(threshold, classStats);

  // -------------------------------
  // 7. Filter vertices based on q(x_i) vs. threshold
  // -------------------------------
  vector<bool> keep(numVertices, false);
  filterVertices(keep, q, threshold, targets, numVertices);

  // -------------------------------
  // 8. Re-index the remaining vertices
  // -------------------------------
  vector<int> newIndex(numVertices, -1);
  int newCounter = reindexVertices(newIndex, keep, numVertices);

  size_t numNewVertices = newCounter;
  cout << "Original vertices: " << numVertices << ", kept vertices: " << numNewVertices << "\n";

  // -------------------------------
  // 9. Write the filtered vertices and targets files
  // -------------------------------
  string filtered_features_file_name_with_path = outputPathFromInputPath(features_file_name_with_path);
  string filtered_targets_file_name_with_path = outputPathFromInputPath(targets_file_name_with_path);

  if (writeFilteredVertices(vertices, keep, filtered_features_file_name_with_path) != 0) {
    return 1;
  }

  if (writeFilteredTargets(targets, keep, filtered_targets_file_name_with_path) != 0) {
    return 1;
  }

  // -------------------------------
  // 10. Write the filtered edges file
  // -------------------------------
  string filtered_edges_file_name_with_path = outputPathFromInputPath(edges_file_name_with_path);

  if (writeFilteredEdges(edges, keep, newIndex, filtered_edges_file_name_with_path) != 0) {
    return 1;
  }

  cout << "Filtering complete. Files written:\n"
     << "  " << filtered_features_file_name_with_path << "\n"
     << "  " << filtered_targets_file_name_with_path << "\n"
     << "  " << filtered_edges_file_name_with_path << "\n";

  return 0;
}
