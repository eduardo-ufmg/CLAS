#include "classify.hpp"

#include <vector>
#include <map>
#include <cmath>
#include <iostream>
#include <numeric>
#include <algorithm>

#include "graphTypes.hpp"

using namespace std;

Expert* getClosestExpert(const Vertex& vertex, const vector<Expert>& experts);
double computeHyperplaneSeparationValue(const Vertex& vertex, const Expert* expert);
int sign(const double value);
ClusterID classifyVertex(const double separationValue);
int insertClassifiedVertexIntoClusterMap(ClusterMap& clusters, const VertexID vertexid, const shared_ptr<Vertex>& vertex, const ClusterID label);

ClassifiedVertices classify(ClusterMap& clusters, const vector<Expert>& experts, VertexMap& vertices)
{
  ClassifiedVertices classifiedVertices;
  
  for (auto& [vertexid, vertexptr] : vertices) {

    const Vertex vertex = *vertexptr;
    
    const Expert* closestExpert = getClosestExpert(vertex, experts);
    const double separationValue = computeHyperplaneSeparationValue(vertex, closestExpert);
    ClusterID label = classifyVertex(separationValue);

    if (insertClassifiedVertexIntoClusterMap(clusters, vertexid, vertexptr, label) != 0) {
      cout << "Could not insert classified vertex into cluster map" << endl;
    }

    classifiedVertices.push_back(make_pair(vertexid, label));

  }

  return classifiedVertices;
}

Expert* getClosestExpert(const Vertex& vertex, const vector<Expert>& experts)
{
  double minDistance = numeric_limits<double>::max();
  Expert* closestExpert = nullptr;

  for (auto& expert : experts) {
    double distance = 0.0;

    for (size_t i = 0; i < vertex.features.size(); i++) {
      distance += pow(vertex.features[i] - expert.midpoint_coordinates[i], 2);
    }

    distance = sqrt(distance);

    if (distance < minDistance) {
      minDistance = distance;
      closestExpert = const_cast<Expert*>(&expert);
    }

  }

  return closestExpert;
}

double computeHyperplaneSeparationValue(const Vertex& vertex, const Expert* expert)
{
  double separationValue = -expert->bias;

  for (size_t i = 0; i < vertex.features.size(); i++) {
    separationValue += vertex.features[i] * expert->differences[i];
  }

  return separationValue;
}

int sign(const double value)
{
  return (value > 0) - (value < 0);
}

ClusterID classifyVertex(const double separationValue)
{
  return sign(separationValue);
}

int insertClassifiedVertexIntoClusterMap(ClusterMap& clusters, const VertexID vertexid, const shared_ptr<Vertex>& vertex, const ClusterID label)
{
  if (clusters.find(label) == clusters.end()) {
    cout << "Error: Could not find cluster with label " << label << endl;
    return -1;
  }

  clusters[label].addVertex(vertexid, vertex);

  return 0;
}
