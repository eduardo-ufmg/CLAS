#include "types.hpp"

#include <cmath>
#include <algorithm>
#include <numeric>

using namespace std;

BaseVertex::BaseVertex(const VertexID id, const Coordinates coordinates)
  : id(id), coordinates(coordinates)
{}

Vertex::Vertex(const VertexID id, const Coordinates coordinates, shared_ptr<Cluster> cluster)
  : BaseVertex(id, coordinates), cluster(cluster), quality(0.0f)
{}

const Coordinates BaseExpert::computeMidpoint(const Edge& edge)
{
  const auto& [v1, v2] = edge;
  const auto& c1 = v1->coordinates;
  const auto& c2 = v2->coordinates;

  Coordinates midpoint(c1.size());

  transform(c1.begin(), c1.end(),
            c2.begin(),
            midpoint.begin(),
            [](const float x, const float y) {
              return (x + y) / 2.0f;
            });

  return midpoint;
}

const NormalVector BaseExpert::computeNormal(const Edge& edge)
{
  const auto& [v1, v2] = edge;
  const auto& c1 = v1->coordinates;
  const auto& c2 = v2->coordinates;

  NormalVector normal(c1.size());

  transform(c1.begin(), c1.end(),
            c2.begin(),
            normal.begin(),
            minus<float>());

  return normal;
}

float BaseExpert::computeBias(const Coordinates& midpoint, const NormalVector& normal)
{
  return inner_product(midpoint.begin(), midpoint.end(),
                       normal.begin(),
                       0.0f);
}

BaseExpert::BaseExpert(const ExpertID id, const Edge edge)
  : id(id), edge(edge),
    midpoint(computeMidpoint(edge)),
    normal(computeNormal(edge)),
    bias(computeBias(midpoint, normal))
{}

BaseExpert::BaseExpert(const ExpertID id, const Coordinates midpoint, const NormalVector normal, const float bias)
  : id(id), edge({nullptr, nullptr}), midpoint(midpoint), normal(normal), bias(bias)
{}

template<typename ExpertType>
std::unique_ptr<ExpertType> BaseExpert::create(const ExpertID id, const Edge edge)
{
  Coordinates midpoint = computeMidpoint(edge);
  NormalVector normal = ExpertType::computeNormal(edge);
  float bias = ExpertType::computeBias(midpoint, normal);

  return std::make_unique<ExpertType>(id, midpoint, normal, bias);
}

// ExpertCHIP implementations
const NormalVector ExpertCHIP::computeNormal(const Edge& edge)
{
  const auto& c2 = edge.second->coordinates;

  const Coordinates cmp = computeMidpoint(edge);

  NormalVector normal(cmp.size());

  transform(cmp.begin(), cmp.end(),
            c2.begin(),
            normal.begin(),
            minus<float>());

  return normal;
}

float ExpertCHIP::computeBias(const Coordinates& midpoint, const NormalVector& normal)
{
  const auto& c2 = edge.second->coordinates;
  
  Coordinates adjustedMidpoint(midpoint.size());

  transform(midpoint.begin(), midpoint.end(),
            c2.begin(),
            adjustedMidpoint.begin(),
            [](const float m, const float c) {
              return (m + c) / 2.0f;
            });

  return inner_product(adjustedMidpoint.begin(), adjustedMidpoint.end(),
                        normal.begin(),
                        0.0f);
}

// ExpertRCHIP implementations
const NormalVector ExpertRCHIP::computeNormal(const Edge& edge)
{
  const auto& [v1, v2] = edge;
  const auto& c1 = v1->coordinates;
  const auto& c2 = v2->coordinates;

  NormalVector normal(c1.size());

  transform(c1.begin(), c1.end(),
            c2.begin(),
            normal.begin(),
            plus<float>());

  return normal;
}

float ExpertRCHIP::computeBias(const Coordinates& midpoint, const NormalVector& normal)
{
  return inner_product(midpoint.begin(), midpoint.end(),
                        normal.begin(),
                        0.0f);
}
