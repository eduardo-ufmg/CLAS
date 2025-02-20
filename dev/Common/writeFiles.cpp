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

ofstream openFileWrite(const string& filename_with_path);

/*

  output format:

  vertex0_id, |, feature0_0, feature0_1, ..., feature0_n, |, cluster0_id, |, adjacent_vertex0_0 - isSupportEdge, adjacent_vertex0_1 - isSupportEdge, ..., adjacent_vertex0_m - isSupportEdge
  vertex1_id, |, feature1_0, feature1_1, ..., feature1_n, |, cluster1_id, |, adjacent_vertex1_0 - isSupportEdge, adjacent_vertex1_1 - isSupportEdge, ..., adjacent_vertex1_m - isSupportEdge
  ...
  vertexk_id, |, featurek_0, featurek_1, ..., featurek_n, |, clusterk_id, |, adjacent_vertexk_0 - isSupportEdge, adjacent_vertexk_1 - isSupportEdge, ..., adjacent_vertexk_m - isSupportEdge

*/

int writeGabrielGraphToFile(const ClusterMap& clusters, const string& filename_with_path)
{
  ofstream outfile = openFileWrite(filename_with_path);
  
  // Iterate over each cluster.
  for (const auto& [clusterid, cluster] : clusters) {
    
    // Iterate over each vertex in the cluster.
    for (const auto& [vertexid, vertexptr] : cluster.vertices) {

      // Write vertex id.
      outfile << vertexid << ", |";
      
      // Write features separated by comma.
      for (const double& f : vertexptr->features) {
        outfile << ", " << f;
      }
      
      // Write cluster id.
      outfile << ", |, " << clusterid << ", |";
      
      // Write adjacent vertices.
      for (const auto& [adjid, isSE] : vertexptr->adjacents) {
        outfile << ", " << adjid << " - " << isSE;
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

int writeVerticesToFile(const ClusterMap& clusters, const string& filename_with_path)
{
  ofstream outfile = openFileWrite(filename_with_path);
  
  // Iterate over each cluster.
  for (const auto& [custerid, cluster] : clusters) {
    
    // Iterate over each vertex in the cluster.
    for (const auto& [_, vertexptr] : cluster.vertices) { (void)_;
      // Write features separated by comma.
      for (const double& f : vertexptr->features) {
        outfile << f << ", ";
      }
      
      // Write cluster id.
      outfile << custerid << "\n";
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
int writeExpertsToFile(const vector<Expert>& experts, const string& filename_with_path)
{
  ofstream outfile = openFileWrite(filename_with_path);
  
  for (const auto& expert : experts) {
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

/*
  output format:

  id0, class0
  id1, class1
  ...
  idk, classk

*/
int writeClassifiedVertices(const ClassifiedVertices& vertices, const string& filename_with_path)
{
  ofstream outfile = openFileWrite(filename_with_path);

  for (const auto& [vertexid, clusterid] : vertices) {
    outfile << vertexid << ", " << clusterid << "\n";
  }

  outfile.close();
  return 0;
}

ofstream openFileWrite(const string& filename_with_path)
{
  ofstream outfile(filename_with_path);

  if (!outfile.is_open()) {
    cerr << "Error: could not open file " << filename_with_path << " for writing." << endl;
    exit(1);
  }
  
  return outfile;
}
