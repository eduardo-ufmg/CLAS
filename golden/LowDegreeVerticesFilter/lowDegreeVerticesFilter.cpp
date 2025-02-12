#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cstdlib>

#include "graphTypes.hpp"
#include "stringHelpers.hpp"
#include "readFiles.hpp"
#include "buildGraph.hpp"
#include "filter.hpp"
#include "writeFiles.hpp"

using namespace std;

const double DEFAULT_DEVIATION_FACTOR = 1.0;

int main(int argc, char* argv[])
{
  if(argc < 4) {
    cerr << "Usage: " << argv[0] << " <features>.csv <targets>.csv <edges>.csv [deviation_factor]" << endl;
    return 1;
  }

  string features_file_name_with_path = argv[1];
  string targets_file_name_with_path = argv[2];
  string edges_file_name_with_path = argv[3];

  double deviation_factor = argc > 4 ? atof(argv[4]) : DEFAULT_DEVIATION_FACTOR;

  vector<Cluster> clusters;

  { // scope to limit the lifetime of raw_vertices, raw_targets, raw_edges and vertices

  vector< vector<double> > raw_vertices;
  if (readVertices(raw_vertices, features_file_name_with_path) != 0) {
    cerr << "Error: Could not read vertices." << endl;
    return 1;
  }

  vector<int> raw_targets;
  if (readTargets(raw_targets, targets_file_name_with_path) != 0) {
    cerr << "Error: Could not read targets." << endl;
    return 1;
  }

  vector< pair<int,int> > raw_edges;
  if (readEdges(raw_edges, edges_file_name_with_path) != 0) {
    cerr << "Error: Could not read edges." << endl;
    return 1;
  }

  if (raw_targets.size() != raw_vertices.size()) {
    cerr << "Error: Number of targets ("
          << raw_targets.size()
          << ") does not match number of vertices ("
          << raw_vertices.size()
          << ")." << endl;
    return 1;
  }

  vector<Vertex> vertices;

  buildVertexVector(vertices, raw_vertices, raw_edges);

  cout << "Number of vertices: " << vertices.size() << endl;

  buildClusters(clusters, vertices, raw_targets);

  cout << "Number of clusters: " << clusters.size() << endl;

  for (auto cluster : clusters) {
    cout << "Cluster " << cluster.id << " has " << cluster.vertices.size() << " vertices." << endl;
  }

  } // end of scope to limit the lifetime of raw_vertices, raw_targets, raw_edges and vertices

  computeVerticesAndClusterQuality(clusters);

  cout << "Clusters Quality Measure and Average Quality" << endl;
  
  for (auto cluster : clusters) {
    cout << "Cluster " << cluster.id << " has quality measure " << cluster.Q.sum_q << " and average quality " << cluster.averageQuality << endl;
  }

  computeClusterDeviation(clusters);

  cout << "Clusters Standard Deviation" << endl;

  for (auto cluster : clusters) {
    cout << "Cluster " << cluster.id << " has standard deviation " << cluster.stdDeviation << endl;
  }

  computeClusterTresholds(clusters, deviation_factor);

  cout << "Clusters Tresholds" << endl;
  for (auto cluster : clusters) {
    cout << "Cluster " << cluster.id << " has treshold " << cluster.treshold << endl;
  }

  filterVertices(clusters);

  cout << "Clusters after filtering" << endl;

  for (auto cluster : clusters) {
    cout << "Cluster " << cluster.id << " has " << cluster.vertices.size() << " vertices." << endl;
  }

  string features_file_name = features_file_name_with_path.substr(features_file_name_with_path.find_last_of("/")+1);
  string dataset_name = features_file_name.substr(0, features_file_name.find_last_of("-"));
  string graph_file_name_with_path = "./output/" + dataset_name + "-graph-filtered.csv";

  if (writeGraphToFile(clusters, graph_file_name_with_path) != 0) {
    cerr << "Error: Could not write graph to file." << endl;
    return 1;
  }

  cout << "Graph written to " << graph_file_name_with_path << endl;

  return 0;
}
