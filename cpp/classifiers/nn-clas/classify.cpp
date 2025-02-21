#include "classify.hpp"

#include <vector>
#include <map>
#include <cmath>
#include <iostream>
#include <numeric>
#include <algorithm>

#include "graphTypes.hpp"
#include "getSEs.hpp"
#include "squaredDistance.hpp"

using namespace std;

ClassifiedVertices classify(const NNsupportEdges& se, VertexMap& vertices)
{
  ClassifiedVertices classifiedVertices;

  for (auto& [vertexid, vertex] : vertices) {
    ClusterID closestVertexClusterid;
    double closestVertexDistance = numeric_limits<double>::max();

    for (const auto& [vapair, vbpair] : se) {
      const auto& [cida, va] = vapair;
      const auto& [cidb, vb] = vbpair;

      double da = squaredDistance(vertex->features, va->features);
      double db = squaredDistance(vertex->features, vb->features);

      if (da < closestVertexDistance) {
        closestVertexDistance = da;
        closestVertexClusterid = cida;
      }

      if (db < closestVertexDistance) {
        closestVertexDistance = db;
        closestVertexClusterid = cidb;
      }

    }

    classifiedVertices.push_back({vertexid, closestVertexClusterid});
  }

  return classifiedVertices;
}
