#ifndef GRAPH_TYPES_HPP
#define GRAPH_TYPES_HPP

#include <vector>
#include <map>

typedef unsigned ID_t;

class Vertex
{
public:
  std::vector<double> features;
  std::vector<ID_t> adjacents;
  ID_t id;
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
  ID_t id;
};

#endif