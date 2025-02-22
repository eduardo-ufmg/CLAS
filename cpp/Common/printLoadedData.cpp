#include "printLoadedData.hpp"

#include <iostream>

using namespace std;

void printClusterVertexQuantity(ClusterMap clusters)
{
  cout << "There are " << clusters.size() << " clusters." << endl;

  for (auto [id, cluster] : clusters) {
    cout << "Cluster " << id << " has " << cluster.vertices.size() << " vertices." << endl;
  }
}

void printClusterStatistics(ClusterMap clusters)
{
  cout << "Cluster statistics:" << endl;

  for (auto [id, cluster] : clusters) {
    cout << "Cluster " << id << ": " << endl;
    cout << "  Q.magnitude: " << cluster.Q.magnitude << endl;
    cout << "  Q.sum_q: " << cluster.Q.sum_q << endl;
    cout << "  averageQuality: " << cluster.averageQuality << endl;
    cout << "  stdDeviation: " << cluster.stdDeviation << endl;
    cout << "  threshold: " << cluster.threshold << endl;
  }
}

void printAdjacencyLists(ClusterMap clusters, int maxCluster, int maxVertexPerCluster)
{
  cout << "Some adjacency lists:" << endl;

  int clusterCount = 0;
  for (auto [_, cluster] : clusters) { (void)_;
    if (clusterCount >= maxCluster) break;
    
    int vertexCount = 0;
    
    for (auto [id, vertex] : cluster.vertices) {
      if (vertexCount >= maxVertexPerCluster) break;
      
      cout << "Vertex " << id << ": ";
      
      for (auto adjacent : vertex.adjacents) {
        cout << adjacent.first << " ";
      }
      cout << endl;
      
      ++vertexCount;
    }
    
    ++clusterCount;
  }
}

void printExpertData(vector<Expert> experts)
{
  cout << "Expert data:" << endl;

  for (auto expert : experts) {
    cout << "Expert " << expert.id << ":" << endl;
    cout << "  Edge: (" << expert.edge.first << ", " << expert.edge.second << ")" << endl;
    cout << "  Differences: ";
    for (auto difference : expert.differences) {
      cout << difference << " ";
    }
    cout << endl;
    cout << "  Midpoint: ";
    for (auto coordinate : expert.midpoint_coordinates) {
      cout << coordinate << " ";
    }
    cout << endl;
    cout << "  Bias: " << expert.bias << endl;
  }
}
