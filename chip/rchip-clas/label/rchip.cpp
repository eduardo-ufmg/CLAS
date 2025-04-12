#include "rchip.hpp"

#include <cmath>
#include <algorithm>
#include <numeric>

#include "squaredDistance.hpp"

using namespace std;

int sign(const double num);
const ExpertRCHIP& getClosestExpert(const Coordinates& point, const Experts& experts);
double computeHyperplaneSeparation(const Coordinates& point, const ExpertRCHIP& expert);
ClusterID labelVertex(const double separation, const chipIDbimap& chipidbimap);

const LabeledVertices rchip(const VerticesToLabel& vertices, const Experts& experts, const chipIDbimap& chipidbimap)
{
  LabeledVertices labeledVertices;

  labeledVertices.reserve(vertices.size());

  for (const auto& vertex : vertices) {

    const ExpertRCHIP& closestExpert = getClosestExpert(vertex.coordinates, experts);
    const double separation = computeHyperplaneSeparation(vertex.coordinates, closestExpert);
    const ClusterID clusterid = labelVertex(separation, chipidbimap);

    labeledVertices.emplace_back(vertex.id, vertex.coordinates, clusterid);
  }

  return labeledVertices;
}

int sign(const double num)
{
  return (num > 0) - (num < 0);
}

const ExpertRCHIP& getClosestExpert(const Coordinates& point, const Experts& experts)
{
  return static_cast<const ExpertRCHIP&>(*min_element(experts.begin(), experts.end(),
                      [&point](const unique_ptr<BaseExpert>& expert1, const unique_ptr<BaseExpert>& expert2) {
                        return squaredDistance(point, static_cast<const ExpertRCHIP&>(*expert1).midpoint) <
                               squaredDistance(point, static_cast<const ExpertRCHIP&>(*expert2).midpoint);
                      })->get());
}

double computeHyperplaneSeparation(const Coordinates& point, const ExpertRCHIP& expert)
{
  return inner_product(point.begin(), point.end(),
                       expert.normal.begin(), -expert.bias);
}

ClusterID labelVertex(const double separation, const chipIDbimap& chipidbimap)
{
  const int chip = sign(separation);
  return chipidbimap.getcid(chip);
}
