#include "rchip.hpp"

#include <cmath>
#include <algorithm>
#include <numeric>
#include <stdexcept>

#include "squaredDistance.hpp"

using namespace std;

int sign(const double num);
const Hyperplane& getClosestHyperplane(const Coordinates& point, const Hyperplanes& hyperplanes);
double computeHyperplaneSeparation(const Coordinates& point, const Hyperplane& hyperplane);
ClusterID labelVertex(const double separation, const chipIDbimap& chipidbimap);

const LabeledVertices rchip(const VerticesToLabel& vertices, const Hyperplanes& hyperplanes, const chipIDbimap& chipidbimap)
{
  LabeledVertices labeledVertices;

  labeledVertices.reserve(vertices.size());

  for (const auto& vertex : vertices) {

    const Hyperplane& closestHyperplane = getClosestHyperplane(vertex.coordinates, hyperplanes);
    const double separation = computeHyperplaneSeparation(vertex.coordinates, closestHyperplane);
    const ClusterID clusterid = labelVertex(separation, chipidbimap);

    labeledVertices.emplace_back(vertex.id, vertex.coordinates, clusterid);
  }

  return labeledVertices;
}

int sign(const double num)
{
  return (num > 0) - (num < 0);
}

const Hyperplane& getClosestHyperplane(const Coordinates& point, const Hyperplanes& hyperplanes)
{
  auto it = min_element(hyperplanes.begin(), hyperplanes.end(),
                        [&point](const Hyperplane& a, const Hyperplane& b) {
                          return squaredDistance(point, a.normal) < squaredDistance(point, b.normal);
                        });

  if (it == hyperplanes.end()) {
    throw runtime_error("No hyperplanes found");
  }

  return *it;
}

double computeHyperplaneSeparation(const Coordinates& point, const Hyperplane& hyperplane)
{
  return inner_product(point.begin(), point.end(),
                       hyperplane.normal.begin(), -hyperplane.bias);
}

ClusterID labelVertex(const double separation, const chipIDbimap& chipidbimap)
{
  const int chip = sign(separation);
  return chipidbimap.getcid(chip);
}
