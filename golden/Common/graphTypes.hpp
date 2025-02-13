#ifndef GRAPH_TYPES_HPP
#define GRAPH_TYPES_HPP

#include <vector>
#include <string>
#include <map>
#include <variant>

typedef unsigned VertexID_t;

class Vertex
{
public:
  std::vector<double> features;
  std::vector<VertexID_t> adjacents;
  VertexID_t id;
  double q;

  bool operator<(const Vertex& other) const;
  bool operator==(const Vertex& other) const;
};

class QualityMeasure
{
public:
  double sum_q;
  double magnitude;
};

class Cluster
{
public:
  std::vector<Vertex> vertices;
  QualityMeasure Q;
  double treshold;
  double averageQuality;
  double stdDeviation;
};

using ClassType = std::variant<unsigned, std::string>;
typedef std::map<ClassType, Cluster> ClusterMap;

#endif