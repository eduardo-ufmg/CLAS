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
