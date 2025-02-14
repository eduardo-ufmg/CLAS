#include "printLoadedData.hpp"

#include <iostream>

using namespace std;

void printClusterInfo(const ClusterMap& clusters)
{
  cout << "There are " << clusters.size() << " clusters." << endl;

  for (auto it = clusters.begin(); it != clusters.end(); ++it) {
    cout << "Cluster " << it->first << " has " << it->second.vertices.size() << " vertices." << endl;
  }
}

void printAdjacencyLists(const ClusterMap& clusters, int maxCluster, int maxVertexPerCluster)
{
  cout << "Some adjacency lists:" << endl;

  int clusterCount = 0;
  for (const auto& clusterPair : clusters) {
    if (clusterCount >= maxCluster) break;
    
    const Cluster& cluster = clusterPair.second;
    int vertexCount = 0;
    
    for (const auto& vertex : cluster.vertices) {
      if (vertexCount >= maxVertexPerCluster) break;
      
      cout << "Vertex " << vertex.id << ": ";
      
      for (const auto& neighbor : vertex.adjacents) {
        cout << neighbor->id << " ";
      }
      cout << endl;
      
      vertexCount++;
    }
    
    clusterCount++;
  }
}
