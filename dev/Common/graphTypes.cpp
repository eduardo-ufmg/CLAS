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

size_t Cluster::addVertex(const VertexID_t vertexid, const shared_ptr<Vertex>& vertex)
{
  this->vertices.emplace(vertexid, vertex);
  vertex->cluster = this;
  return vertices.size();
}

size_t Cluster::removeVertex(VertexID_t vertexid)
{
  this->vertices.at(vertexid)->cluster = nullptr;
  this->vertices.erase(vertexid);
  return vertices.size();
}
