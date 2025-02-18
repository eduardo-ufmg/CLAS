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

using VertexID_t = int;

class Vertex
{
public:
  std::vector<double> features;
  std::vector< std::pair<VertexID_t, bool> > adjacents;
  double q;
  const Cluster* cluster;
  Vertex(std::vector<double> features, const Cluster* cluster) : features(features), cluster(cluster) {}
  Vertex() {}
};

using VertexMap = std::map<VertexID_t, std::shared_ptr<Vertex> >;

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
};

using ClassType = std::variant<int, std::string>;
using ClusterMap = std::map<ClassType, Cluster>;

template<typename... Ts>
std::enable_if_t<(sizeof...(Ts) > 0), std::ostream&>
operator<<(std::ostream& os, const std::variant<Ts...>& var) {
  std::visit([&os](const auto& value) { os << value; }, var);
  return os;
}

using Edge = std::pair<VertexID_t, VertexID_t>;
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

#endif