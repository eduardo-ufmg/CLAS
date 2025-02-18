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

  vertex0_id, |, feature0_0, feature0_1, ..., feature0_n, |, cluster0_id, |, adjacent_vertex0_0 - isSupportEdge, adjacent_vertex0_1 - isSupportEdge, ..., adjacent_vertex0_m - isSupportEdge
  vertex1_id, |, feature1_0, feature1_1, ..., feature1_n, |, cluster1_id, |, adjacent_vertex1_0 - isSupportEdge, adjacent_vertex1_1 - isSupportEdge, ..., adjacent_vertex1_m - isSupportEdge
  ...
  vertexk_id, |, featurek_0, featurek_1, ..., featurek_n, |, clusterk_id, |, adjacent_vertexk_0 - isSupportEdge, adjacent_vertexk_1 - isSupportEdge, ..., adjacent_vertexk_m - isSupportEdge

*/

int writeGabrielGraphToFile(const ClusterMap& clusters, const std::string& file_name_with_path)
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
    for (const auto& [vertexID, vertexptr] : cluster.vertices) {
      // Write vertex id.
      outfile << vertexID << ", |";
      
      // Write features separated by comma.
      for (const double& f : vertexptr->features) {
        outfile << ", " << f;
      }
      
      // Write cluster id.
      outfile << ", |, " << clusterKey << ", |";
      
      // Write adjacent vertices.
      for (const auto& adj : vertexptr->adjacents) {
        outfile << ", " << adj.first << " - " << adj.second;
      }
      
      outfile << "\n";
    }
  }
  
  outfile.close();
  return 0;
}

/*

output format:

feature0_0, feature0_1, ..., feature0_n, cluster0_id
feature1_0, feature1_1, ..., feature1_n, cluster1_id
...
featurek_0, featurek_1, ..., featurek_n, clusterk_id

*/

int writeVerticesToFile(const ClusterMap& clusters, const std::string& file_name_with_path)
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
    for (const auto& [vertexID, vertexptr] : cluster.vertices) {
      // Write features separated by comma.
      for (const double& f : vertexptr->features) {
        outfile << f << ", ";
      }
      
      // Write cluster id.
      outfile << clusterKey << "\n";
    }
  }
  
  outfile.close();
  return 0;
}

/*

output format:

id0, |, v00, v01, |, diff_coord00, diff_coord01, ..., diff_coord0n, |, midpoint_coord00, midpoint_coord01, ..., midpoint_coord0n, |, bias0
id1, |, v10, v11, |, diff_coord10, diff_coord11, ..., diff_coord1n, |, midpoint_coord10, midpoint_coord11, ..., midpoint_coord1n, |, bias1
...
idk, |, vk0, vk1, |, diff_coordk0, diff_coordk1, ..., diff_coordkn, |, midpoint_coordk0, midpoint_coordk1, ..., midpoint_coordkn, |, biask

*/
int writeExpertsToFile(const vector<Expert>& experts, const std::string& file_name_with_path)
{
  ofstream outfile(file_name_with_path);

  if (!outfile.is_open()) {
    cerr << "Error: Unable to open file " << file_name_with_path << " for writing." << endl;
    return -1;
  }
  
  for (const Expert& expert : experts) {
    // Write expert id.
    outfile << expert.id << ", |, ";
    
    // Write edge vertices.
    outfile << expert.edge.first << ", " << expert.edge.second << ", |";
    
    // Write diff_coord.
    for (const double& diff_coord : expert.differences) {
      outfile << ", " << diff_coord;
    }

    outfile << ", |";
    
    // Write midpoint_coord.
    for (const double& midpoint_coord : expert.midpoint_coordinates) {
      outfile << ", " << midpoint_coord;
    }
    
    // Write bias.
    outfile << ", |, " << expert.bias << "\n";
  }
  
  outfile.close();
  return 0;
}

