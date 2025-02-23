#include "graphTypes.hpp"

#include <vector>
#include <map>

using namespace std;

Vertex::Vertex(const Coordinates features, const AdjacencyVector adjacents, Cluster* const cluster)
  : features(features), adjacents(adjacents), cluster(cluster)
{}

size_t Cluster::addVertex(const VertexID vertexid, Vertex vertex)
{
  this->vertices.emplace(vertexid, vertex);
  vertex.cluster = this;
  return vertices.size();
}

size_t Cluster::removeVertex(const VertexID vertexid)
{
  this->vertices.at(vertexid).cluster = nullptr;
  this->vertices.erase(vertexid);
  return vertices.size();
}

Expert::Expert(const Edge edge, const ExpertID id)
  : edge(edge), id(id)
{}

Expert::Expert(const Edge edge, const ExpertDifferences differences, const Coordinates mpCoordinates, const double bias, const ExpertID id)
  : edge(edge), differences(differences), mpCoordinates(mpCoordinates), bias(bias), id(id)
{}
