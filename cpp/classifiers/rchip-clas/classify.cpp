#include "classify.hpp"

#include <vector>
#include <map>
#include <cmath>
#include <iostream>
#include <numeric>
#include <algorithm>

#include "graphTypes.hpp"

using namespace std;

double squaredDistance(const vector<double>& a, const vector<double>& b);
const Expert* getClosestExpert(const Vertex& vertex, const vector<Expert>& experts);
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

double squaredDistance(const std::vector<double>& a, const std::vector<double>& b)
{
  if (a.size() != b.size()) {
   return numeric_limits<double>::infinity();
  }

  return inner_product(a.begin(), a.end(),
    b.begin(), 0.0, plus<double>(),
    [](double x, double y) {
      return (x - y) * (x - y);
    });
}

const Expert* getClosestExpert(const Vertex& vertex, const vector<Expert>& experts)
{
  auto it = min_element(experts.begin(), experts.end(),
    [&vertex](const Expert& a, const Expert& b) {
      return squaredDistance(vertex.features, a.midpoint_coordinates) < squaredDistance(vertex.features, b.midpoint_coordinates);
    });

  return (it != experts.end()) ? &(*it) : nullptr;
}

double computeHyperplaneSeparationValue(const Vertex& vertex, const Expert* expert)
{
  return inner_product(vertex.features.begin(), vertex.features.end(),
    expert->differences.begin(), -expert->bias);
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
