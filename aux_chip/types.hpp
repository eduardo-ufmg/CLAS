#ifndef TYPES_HPP
#define TYPES_HPP

#include <vector>
#include <string>
#include <memory>
#include <map>

class Vertex;
class Cluster;

using VertexID = int;
using Coordinates = std::vector<float>;
using AdjacentVertex = std::pair<const Vertex *, bool>; // second: is support edge
using AdjacencyList = std::vector<AdjacentVertex>;

class BaseVertex
{
public:
  VertexID id;
  Coordinates coordinates;

  BaseVertex(const VertexID id, const Coordinates coordinates);  
};

class Vertex : public BaseVertex
{
public:
  std::shared_ptr<Cluster> cluster;
  
  AdjacencyList adjacencyList;
  float quality;

  Vertex(const VertexID id, const Coordinates coordinates, std::shared_ptr<Cluster> cluster = nullptr);
};

using Vertices = std::vector<Vertex>;

using Edge = std::pair<const Vertex * const, const Vertex * const>;
using ExpertID = int;
using NormalVector = std::vector<float>;

class BaseExpert
{
protected:
  virtual const NormalVector computeNormal(const Edge& edge);
  virtual float computeBias(const Coordinates& midpoint, const NormalVector& normal);

  static const Coordinates computeMidpoint(const Edge& edge);

public:
  const ExpertID id;
  const Edge edge;

  const Coordinates midpoint;
  const NormalVector normal;
  const float bias;

  BaseExpert(const ExpertID id, const Edge edge);
  BaseExpert(const ExpertID id, const Coordinates midpoint, const NormalVector normal, const float bias);

  template<typename ExpertType>
  static std::unique_ptr<ExpertType> create(const ExpertID id, const Edge edge);

  virtual ~BaseExpert() = default;
};

class ExpertCHIP : public BaseExpert
{
public:
  using BaseExpert::BaseExpert;

  const NormalVector computeNormal(const Edge& edge) override;
  float computeBias(const Coordinates& midpoint, const NormalVector& normal) override;
};

class ExpertRCHIP : public BaseExpert
{
public:
  using BaseExpert::BaseExpert;

  const NormalVector computeNormal(const Edge& edge) override;
  float computeBias(const Coordinates& midpoint, const NormalVector& normal) override;
};

class ExpertPred : public BaseExpert
{
public:
  ExpertPred(const ExpertID id, const Coordinates& midpoint, const NormalVector& normal, const float bias)
    : BaseExpert(id, midpoint, normal, bias) {}
};

using Experts = std::vector<std::unique_ptr<BaseExpert>>;

#endif // TYPES_HPP
