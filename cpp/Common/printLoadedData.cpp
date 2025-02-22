#include "printLoadedData.hpp"

#include <iostream>

using namespace std;

void printClusterVertexQuantity(const ClusterMap& clusters)
{
  cout << "There are " << clusters.size() << " clusters." << endl;

  for (const auto& [id, cluster] : clusters) {
    cout << "Cluster " << id << " has " << cluster.vertices.size() << " vertices." << endl;
  }
}

void printClusterStatistics(const ClusterMap& clusters)
{
  cout << "Cluster statistics:" << endl;

  for (const auto& [id, cluster] : clusters) {
    cout << "Cluster " << id << ": " << endl;
    cout << "  Q.magnitude: " << cluster.Q.magnitude << endl;
    cout << "  Q.sum_q: " << cluster.Q.sum_q << endl;
    cout << "  averageQuality: " << cluster.averageQuality << endl;
    cout << "  stdDeviation: " << cluster.stdDeviation << endl;
    cout << "  threshold: " << cluster.threshold << endl;
  }
}

void printAdjacencyLists(const ClusterMap& clusters, const size_t maxCluster, const size_t maxVertexPerCluster)
{
  cout << "Some adjacency lists:" << endl;

  size_t clusterCount = 0;
  for (const auto& [_, cluster] : clusters) { (void)_;
    if (clusterCount >= maxCluster) break;
    
    size_t vertexCount = 0;
    
    for (const auto& [id, vertex] : cluster.vertices) {
      if (vertexCount >= maxVertexPerCluster) break;
      
      cout << "Vertex " << id << ": ";
      
      for (const auto& adjacent : vertex.adjacents) {
        cout << adjacent.first << " ";
      }
      cout << endl;
      
      ++vertexCount;
    }
    
    ++clusterCount;
  }
}

void printExpertData(const Experts& experts)
{
  cout << "Expert data:" << endl;

  for (const auto& expert : experts) {
    cout << "Expert " << expert.id << ":" << endl;
    cout << "  Edge: (" << expert.edge.first << ", " << expert.edge.second << ")" << endl;
    cout << "  Differences: ";
    for (const auto& difference : expert.differences) {
      cout << difference << " ";
    }
    cout << endl;
    cout << "  Midpoint: ";
    for (const auto& coordinate : expert.mpCoordinates) {
      cout << coordinate << " ";
    }
    cout << endl;
    cout << "  Bias: " << expert.bias << endl;
  }
}
