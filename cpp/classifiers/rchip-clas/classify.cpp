#include "classify.hpp"

#include <vector>
#include <map>
#include <cmath>
#include <iostream>
#include <numeric>
#include <algorithm>

#include "graphTypes.hpp"
#include "squaredDistance.hpp"

using namespace std;

Expert* getClosestExpert(Vertex vertex, vector<Expert> experts);
double computeHyperplaneSeparationValue(Vertex vertex, Expert* expert);
int sign(double value);
ClusterID classifyVertex(double separationValue);
int insertClassifiedVertexIntoClusterMap(ClusterMap clusters, VertexID vertexid, shared_ptr<Vertex> vertex, ClusterID label);

ClassifiedVertices classify(ClusterMap clusters, vector<Expert> experts, VertexMap vertices)
{
  ClassifiedVertices classifiedVertices;
  
  for (auto [vertexid, vertexptr] : vertices) {

    Vertex vertex = *vertexptr;
    
    Expert* closestExpert = getClosestExpert(vertex, experts);
    double separationValue = computeHyperplaneSeparationValue(vertex, closestExpert);
    ClusterID label = classifyVertex(separationValue);

    if (insertClassifiedVertexIntoClusterMap(clusters, vertexid, vertexptr, label) != 0) {
      cout << "Could not insert classified vertex into cluster map" << endl;
    }

    classifiedVertices.push_back(make_pair(vertexid, label));

  }

  return classifiedVertices;
}

Expert* getClosestExpert(Vertex vertex, vector<Expert> experts)
{
  auto it = min_element(experts.begin(), experts.end(),
    [vertex](Expert a, Expert b) {
      return squaredDistance(vertex.features, a.midpoint_coordinates) < squaredDistance(vertex.features, b.midpoint_coordinates);
    });

  return (it != experts.end()) ? &(*it) : nullptr;
}

double computeHyperplaneSeparationValue(Vertex vertex, Expert* expert)
{
  return inner_product(vertex.features.begin(), vertex.features.end(),
    expert->differences.begin(), -expert->bias);
}

int sign(double value)
{
  return (value > 0) - (value < 0);
}

ClusterID classifyVertex(double separationValue)
{
  return sign(separationValue);
}

int insertClassifiedVertexIntoClusterMap(ClusterMap clusters, VertexID vertexid, shared_ptr<Vertex> vertex, ClusterID label)
{
  if (clusters.find(label) == clusters.end()) {
    cout << "Error: Could not find cluster with label " << label << endl;
    return -1;
  }

  clusters[label].addVertex(vertexid, vertex);

  return 0;
}
