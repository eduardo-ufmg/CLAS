#include "types.hpp"

#include <cmath>
#include <algorithm>
#include <numeric>

using namespace std;

BaseVertex::BaseVertex(const VertexID id, const Coordinates& coordinates)
  : id(id), coordinates(coordinates)
{}

Vertex::Vertex(const VertexID id, const Coordinates& coordinates, shared_ptr<Cluster> cluster)
  : BaseVertex(id, coordinates), cluster(cluster), quality(0.0f)
{}

Cluster::Cluster(const ClusterID id)
  : id(id), sumq(0.0f), magq(0), online_avgq(0.0f), sumDeltaSq(0.0f), online_stdq(0.0f), threshold(0.0f)
{}

void Cluster::reset()
{
  sumq = 0.0f;
  magq = 0;
  online_avgq = 0.0f;
  sumDeltaSq = 0.0f;
  online_stdq = 0.0f;
  threshold = 0.0f;
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

void Cluster::computeThreshold(const float tolerance)
{
  threshold = online_avgq - tolerance * online_stdq;
}

const Coordinates Hyperplane::computeMidpoint(const Edge& edge) {
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

const NormalVector Hyperplane::computeNormal(const Edge& edge)
{
  const auto& c1 = edge.first->coordinates;
  const auto& c2 = edge.second->coordinates;

  NormalVector normal(c1.size());

  transform(c1.begin(), c1.end(),
            c2.begin(),
            normal.begin(),
            minus<float>());

  return normal;
}

float Hyperplane::computeBias(const Coordinates& midpoint, const NormalVector& normal)
{
  return inner_product(midpoint.begin(), midpoint.end(),
                        normal.begin(),
                        0.0f);
}

Hyperplane::Hyperplane(const HyperplaneID id, const Coordinates& edgeMidpoint, const NormalVector& normal, const float bias)
  : id(id), edgeMidpoint(edgeMidpoint), normal(normal), bias(bias)
{}

Hyperplane::Hyperplane(const HyperplaneID id, const Edge& edge)
  : id(id), edge(edge), edgeMidpoint(computeMidpoint(edge)), normal(computeNormal(edge)), bias(computeBias(edgeMidpoint, normal))
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
