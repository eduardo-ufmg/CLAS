#include "rchip.hpp"

#include <cmath>
#include <algorithm>
#include <numeric>

#include "squaredDistance.hpp"

using namespace std;

int sign(const double num);
const Expert& getClosestExpert(const Coordinates& point, const Experts& experts);
double computeHyperplaneSeparation(const Coordinates& point, const Expert& expert);
ClusterID labelVertex(const double separation);

const LabeledVertices rchip(const VerticesToLabel& vertices, const Experts& experts)
{
  LabeledVertices labeledVertices;

  labeledVertices.reserve(vertices.size());

  for (const auto& vertex : vertices) {

    const Expert& closestExpert = getClosestExpert(vertex.coordinates, experts);
    const double separation = computeHyperplaneSeparation(vertex.coordinates, closestExpert);
    const ClusterID clusterid = labelVertex(separation);

    labeledVertices.emplace_back(vertex.id, vertex.coordinates, clusterid);
  }

  return labeledVertices;
}

int sign(const double num)
{
  return (num > 0) - (num < 0);
}

const Expert& getClosestExpert(const Coordinates& point, const Experts& experts)
{
  return *min_element(experts.begin(), experts.end(),
                      [&point](const Expert& expert1, const Expert& expert2) {
                        return squaredDistance(point, expert1.midpoint) < squaredDistance(point, expert2.midpoint);
                      });
}

double computeHyperplaneSeparation(const Coordinates& point, const Expert& expert)
{
  return inner_product(point.begin(), point.end(),
                       expert.differences.begin(), -expert.bias);
}

ClusterID labelVertex(const double separation)
{
  return sign(separation);
}
