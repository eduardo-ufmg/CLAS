#ifndef GRAPH_TYPES_HPP
#define GRAPH_TYPES_HPP

#include <vector>
#include <string>
#include <map>
#include <set>
#include <variant>
#include <memory>

class Vertex;
class Cluster;

using VertexID = int;
using AdjacencyVector = std::vector< std::pair<VertexID, bool> >;

class Vertex
{
public:
  std::vector<double> features;
  AdjacencyVector adjacents;
  double q;
  Cluster* cluster;
  Vertex(std::vector<double> features = {}, AdjacencyVector adjacents = {}, Cluster* cluster = nullptr);
};

using VertexMap = std::map<VertexID, std::shared_ptr<Vertex> >;

class QualityMeasure
{
public:
  double sum_q;
  double magnitude;
};

class Cluster
{
public:
  VertexMap vertices;
  QualityMeasure Q;
  double threshold;
  double averageQuality;
  double stdDeviation;
  size_t addVertex(const VertexID vertexid, const std::shared_ptr<Vertex>& vertex);
  size_t removeVertex(VertexID vertexid);
};

using ClusterID = std::variant<int, std::string>;
using ClusterMap = std::map<ClusterID, Cluster>;

template<typename... Ts>
std::enable_if_t<(sizeof...(Ts) > 0), std::ostream&>
operator<<(std::ostream& os, const std::variant<Ts...>& var) {
  std::visit([&os](const auto& value) { os << value; }, var);
  return os;
}

using Edge = std::pair<VertexID, VertexID>;
using SupportEdges = std::set<Edge>;
using EdgeVertices = std::pair<const Vertex*, const Vertex*>;

class Expert
{
public:
  Edge edge;
  std::vector<double> differences;
  std::vector<double> midpoint_coordinates;
  double bias;
  unsigned id;
};

using ClassifiedVertices = std::vector< std::pair<VertexID, ClusterID> >;

#endif