#include "graphTypes.hpp"

#include <vector>
#include <map>

using namespace std;

Vertex::Vertex(vector<double> features, AdjacencyVector adjacents, Cluster* cluster)
{
  this->features = features;
  this->adjacents = adjacents;
  this->cluster = cluster;
}

size_t Cluster::addVertex(VertexID vertexid, Vertex vertex)
{
  this->vertices.emplace(vertexid, vertex);
  vertex.cluster = this;
  return vertices.size();
}

size_t Cluster::removeVertex(VertexID vertexid)
{
  this->vertices.at(vertexid).cluster = nullptr;
  this->vertices.erase(vertexid);
  return vertices.size();
}
