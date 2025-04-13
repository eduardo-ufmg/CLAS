#include "chip.hpp"

#include <cmath>
#include <algorithm>
#include <numeric>
#include <stdexcept>

using namespace std;

using Distances = vector<double>;
using MaxDistance = double;
using DistancePair = pair<const Distances, const MaxDistance>;

using Weights = vector<double>;

using HyperplaneDecision = double;

int sign(const double num);
const DistancePair computeDistances(const Coordinates& point, const Hyperplanes& hyperplanes);
Weights computeWeights(const DistancePair& distpair);
void normalizeWeights(Weights& weights);
const Weights computeNormalizedWeights(const DistancePair& distpair);
double computeDecisionSum(const Coordinates& point, const Hyperplanes& hyperplanes, const Weights& weights);
ClusterID labelVertex(const double decision_sum, const chipIDbimap& chipidbimap);

const LabeledVertices chip(const VerticesToLabel& vertices, const Hyperplanes& hyperplanes, const chipIDbimap& chipidbimap)
{
  LabeledVertices labeledVertices;

  labeledVertices.reserve(vertices.size());

  for (const auto& vertex : vertices) {

    const DistancePair distpair = computeDistances(vertex.coordinates, hyperplanes);
    const Weights weights = computeNormalizedWeights(distpair);
    const double decision_sum = computeDecisionSum(vertex.coordinates, hyperplanes, weights);
    const ClusterID clusterid = labelVertex(decision_sum, chipidbimap);

    labeledVertices.emplace_back(vertex.id, vertex.coordinates, clusterid);

  }

  return labeledVertices;
}

int sign(const double num)
{
  return (num > 0) - (num < 0);
}

const DistancePair computeDistances(const Coordinates& point, const Hyperplanes& hyperplanes)
{
  Distances distances;

  distances.reserve(hyperplanes.size());

  double maxDistance = 0.0;

  for (const auto& hyperplane : hyperplanes) {

    const double sqDistance = inner_product(point.begin(), point.end(),
                                            hyperplane.edgeMidpoint.begin(),
                                            0.0, plus<double>(),
                                            [](const double a, const double b) {
                                              return (a - b) * (a - b);
                                            });

    distances.push_back(sqDistance);

    if (sqDistance > maxDistance) {
      maxDistance = sqDistance;
    }

  }

  return make_pair(distances, maxDistance);
}

Weights computeWeights(const DistancePair& distpair)
{
  const Distances& distances = distpair.first;
  const MaxDistance& maxDistance = distpair.second;

  Weights weights(distances.size());

  transform(distances.begin(), distances.end(),
            weights.begin(),
            [&maxDistance](double distance) {
              return exp(- maxDistance * maxDistance / distance);
            });

  return weights;
}

void normalizeWeights(Weights& weights)
{
  const double sum = accumulate(weights.begin(), weights.end(), 0.0);

  if (sum == 0.0) {

    fill(weights.begin(), weights.end(), 1.0 / weights.size());

  } else {

    for (auto& weight : weights) {
      weight /= sum;
    }

  }

}

const Weights computeNormalizedWeights(const DistancePair& distpair)
{
  Weights weights = computeWeights(distpair);

  normalizeWeights(weights);

  return weights;
}

double computeDecisionSum(const Coordinates& point, const Hyperplanes& hyperplanes, const Weights& weights)
{
  vector<HyperplaneDecision> decisions(hyperplanes.size());

  transform(hyperplanes.begin(), hyperplanes.end(),
            weights.begin(),
            decisions.begin(),
            [&point](const auto& hyperplane, const double weight) {

              const double dotProduct = inner_product(point.begin(), point.end(),
                                                      hyperplane.normal.begin(), 0.0);

              return weight * (dotProduct - hyperplane.bias);
            });

  return accumulate(decisions.begin(), decisions.end(), 0.0);
}

ClusterID labelVertex(const double decision_sum, const chipIDbimap& chipidbimap)
{
  const int chip = sign(decision_sum);
  return chipidbimap.getcid(chip);
}