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

using Coordinates = std::vector< double>;

using VertexID = int;
using AdjacencyVector = std::vector< std::pair<const VertexID, const bool> >;

class Vertex
{
public:
  const Coordinates features;
  AdjacencyVector adjacents;
  double q;
  Cluster* cluster;
  Vertex(const Coordinates features = {}, const AdjacencyVector adjacents = {}, Cluster * const cluster = nullptr);
};

using VertexMap = std::map<VertexID, Vertex>;

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
  size_t addVertex(const VertexID vertexid, Vertex vertex);
  size_t removeVertex(const VertexID vertexid);
};

using ClusterID = std::variant<int, std::string>;
using ClusterMap = std::map<ClusterID, Cluster>;

template<typename... Ts>
std::enable_if_t<(sizeof...(Ts) > 0), std::ostream&>
operator<<(std::ostream& os, const std::variant<Ts...>& var) {
  std::visit([&os](const auto& value) { os << value; }, var);
  return os;
}

using Edge = std::pair<const VertexID, const VertexID>;
using SupportEdges = std::set<Edge>;
using EdgeVertices = std::pair<Vertex * const, Vertex * const>;

using ExpertDifferences = std::vector<double>;
using ExpertID = unsigned;

class Expert
{
public:
  const Edge edge;
  ExpertDifferences differences;
  Coordinates mpCoordinates;
  double bias;
  const ExpertID id;
  Expert(const Edge edge, const ExpertID id);
  Expert(const Edge edge, const ExpertDifferences differences, const Coordinates mpCoordinates, const double bias, const ExpertID id);
};

using Experts = std::vector<Expert>;

using ClassifiedVertices = std::vector< std::pair<const VertexID, const ClusterID> >;

#endif // GRAPH_TYPES_HPP
