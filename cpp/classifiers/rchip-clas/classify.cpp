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

Expert* getClosestExpert(const Vertex& vertex, Experts& experts);
double computeHyperplaneSeparationValue(const Vertex& vertex, const Expert& expert);
int sign(const double value);
ClusterID classifyVertex(const double separationValue);
int insertClassifiedVertexIntoClusterMap(ClusterMap& clusters, const VertexID vertexid, Vertex& vertex, const ClusterID label);

const ClassifiedVertices classify(ClusterMap& clusters, Experts& experts, VertexMap& vertices)
{
  ClassifiedVertices classifiedVertices;
  
  for (auto& [vertexid, vertex] : vertices) {
    
    const Expert * const closestExpert = getClosestExpert(vertex, experts);
    const double separationValue = computeHyperplaneSeparationValue(vertex, *closestExpert);
    const ClusterID label = classifyVertex(separationValue);

    if (insertClassifiedVertexIntoClusterMap(clusters, vertexid, vertex, label) != 0) {
      cout << "Could not insert classified vertex into cluster map" << endl;
    }

    classifiedVertices.push_back(make_pair(vertexid, label));

  }

  return classifiedVertices;
}

Expert* getClosestExpert(const Vertex& vertex, Experts& experts)
{
  const auto it = min_element(experts.begin(), experts.end(),
    [&vertex](const Expert& a, const Expert& b) {
      return squaredDistance(vertex.features, a.mpCoordinates) < squaredDistance(vertex.features, b.mpCoordinates);
    });

  return (it != experts.end()) ? &(*it) : nullptr;
}

double computeHyperplaneSeparationValue(const Vertex& vertex, const Expert& expert)
{
  return inner_product(vertex.features.begin(), vertex.features.end(),
    expert.differences.begin(), -expert.bias);
}

int sign(const double value)
{
  return (value > 0) - (value < 0);
}

ClusterID classifyVertex(const double separationValue)
{
  return sign(separationValue);
}

int insertClassifiedVertexIntoClusterMap(ClusterMap& clusters, const VertexID vertexid, Vertex& vertex, const ClusterID label)
{
  if (clusters.find(label) == clusters.end()) {
    cout << "Error: Could not find cluster with label " << label << endl;
    return -1;
  }

  clusters[label].addVertex(vertexid, vertex);

  return 0;
}
