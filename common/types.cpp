#include "types.hpp"

#include <cmath>
#include <algorithm>
#include <numeric>

using namespace std;

BaseVertex::BaseVertex(const VertexID id, const Coordinates& coordinates)
  : id(id), coordinates(coordinates)
{}

Vertex::Vertex(const VertexID id, const Coordinates& coordinates, std::shared_ptr<Cluster> cluster)
  : BaseVertex(id, coordinates), cluster(cluster), quality(0.0f)
{}

Cluster::Cluster(const ClusterID id)
  : id(id), sumq(0.0f), magq(0), online_avgq(0.0f), sumDeltaSq(0.0f), online_stdq(0.0f), treshold(0.0f)
{}

void Cluster::reset()
{
  sumq = 0.0f;
  magq = 0;
  online_avgq = 0.0f;
  sumDeltaSq = 0.0f;
  online_stdq = 0.0f;
  treshold = 0.0f;
}

void Cluster::accumQ_updateStats(const float q)
{
  sumq += q;
  ++ magq;

  const float delta = q - online_avgq;

  online_avgq += delta / magq;

  const float delta2 = delta - online_avgq;

  sumDeltaSq += delta * delta2;

  if (magq > 1) {
    online_stdq = sqrt(sumDeltaSq / (magq - 1));
  }
}

void Cluster::computeTreshold(const float tolerance)
{
  treshold = online_avgq - tolerance * online_stdq;
}

const Coordinates BaseExpert::computeMidpoint(const Edge& edge) {
  const auto& [v1, v2] = edge;
  const auto& c1 = v1->coordinates;
  const auto& c2 = v2->coordinates;

  Coordinates midpoint(c1.size());
  
  std::transform(c1.begin(), c1.end(),
                  c2.begin(),
                  midpoint.begin(),
                  [](const float x, const float y) {
                    return (x + y) / 2.0f;
                  });

  return midpoint;
}

BaseExpert::BaseExpert(const ExpertID id, const Edge& edge, const Coordinates& midpoint, const NormalVector& normal, const float bias)
  : id(id), edge(edge), midpoint(midpoint), normal(normal), bias(bias)
{}

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

float ExpertCHIP::computeBias(const Edge& edge, const Coordinates& midpoint, const NormalVector& normal)
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

// ExpertCHIP constructor
ExpertCHIP::ExpertCHIP(const ExpertID id, const Edge& edge)
  : ExpertCHIP(id, edge, BaseExpert::computeMidpoint(edge), ExpertCHIP::computeNormal(edge))
{}

ExpertCHIP::ExpertCHIP(const ExpertID id, const Edge& edge, const Coordinates& midpoint, const NormalVector& normal)
  : BaseExpert(id, edge, midpoint, normal, ExpertCHIP::computeBias(edge, midpoint, normal))
{}

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

float ExpertRCHIP::computeBias(const Edge& edge, const Coordinates& midpoint, const NormalVector& normal)
{
  return inner_product(midpoint.begin(), midpoint.end(),
                        normal.begin(),
                        0.0f);
}

// ExpertRCHIP constructor
ExpertRCHIP::ExpertRCHIP(const ExpertID id, const Edge& edge)
  : ExpertRCHIP(id, edge, BaseExpert::computeMidpoint(edge), ExpertRCHIP::computeNormal(edge))
{}

ExpertRCHIP::ExpertRCHIP(const ExpertID id, const Edge& edge, const Coordinates& midpoint, const NormalVector& normal)
  : BaseExpert(id, edge, midpoint, normal, ExpertRCHIP::computeBias(edge, midpoint, normal))
{}

// ExpertPred constructor
ExpertPred::ExpertPred(const ExpertID id, const Coordinates& midpoint, const NormalVector& normal, const float bias)
  : BaseExpert(id, edge, midpoint, normal, bias)
{}

SupportVertex::SupportVertex(const VertexID id, const Coordinates& coordinates, const ClusterID clusterid)
  : BaseVertex(id, coordinates), clusterid(clusterid)
{}

VertexToLabel::VertexToLabel(const VertexID id, const Coordinates& coordinates, const ClusterID expectedclusterid)
  : BaseVertex(id, coordinates), expectedclusterid(expectedclusterid)
{}

LabeledVertex::LabeledVertex(const VertexID id, const Coordinates coordinates, const ClusterID clusterid)
  : BaseVertex(id, coordinates), clusterid(clusterid)
{}

void chipIDbimap::insert(const ClusterID& cid, const int chip)
{
  cidtochip.emplace(cid, chip);
  chiptocid.emplace(chip, cid);
}

int chipIDbimap::getchip(const ClusterID& cid) const
{
  return cidtochip.at(cid);
}

const ClusterID& chipIDbimap::getcid(const int chip) const
{
  return chiptocid.at(chip);
}

const cIDtochipMap& chipIDbimap::getcidtochip() const
{
  return cidtochip;
}

const chiptocIDMap& chipIDbimap::getchiptocid() const
{
  return chiptocid;
}
