#include "label.hpp"

#include <vector>
#include <map>
#include <cmath>
#include <iostream>
#include <numeric>
#include <algorithm>

#include "graphTypes.hpp"

using namespace std;

using Distances = vector<double>;
using MaxDistance = double;
using DistancePair = pair<Distances, MaxDistance>;

using Weights = vector<double>;

using ExpertDecision = double;

int sign(const double num);
const DistancePair computeDistances(const Coordinates& point, const Experts& experts);
Weights computeWeights(const Distances& distances, const MaxDistance maxDistance);
void normalizeWeights(Weights& weights);
const Weights computeNormalizedWeights(const Distances& distances, const MaxDistance maxDistance);
double computeDecisionSum(const Coordinates& point, const Experts& experts, const Weights& weights);
ClusterID labelVertex(const double decision_sum);
int insertLabeledVertexIntoClusterMap(ClusterMap& clusters, const VertexID vertexid, Vertex& vertex, const ClusterID label);

const LabeledVertices label(ClusterMap& clusters, const Experts& experts, VertexMap& vertices)
{
  LabeledVertices labeledVertices;
  
  for (auto& [vertexid, vertex] : vertices) {

    const Coordinates& point = vertex.features;

    // 1. Compute distances from point to each expert's midpoint
    const auto [distances, maxDistance] = computeDistances(point, experts);

    // 2. Compute expert weights using the gating function:
    //    c_l(point) = exp( - maxDistance^2 / D(point, pl) )
    const Weights weights = computeNormalizedWeights(distances, maxDistance);

    // 3. Compute the weighted sum of expert decisions.
    // For each expert, h_l(point) = sign( dot(point, expert.differences) - expert.bias )
    const double decision_sum = computeDecisionSum(point, experts, weights);

    // Final classification: f(point) = sign( sum_l c_l(point) * h_l(point) )
    const ClusterID label = labelVertex(decision_sum);

    // 4. Update the vertex's cluster assignment.
    if (insertLabeledVertexIntoClusterMap(clusters, vertexid, vertex, label) != 0) {
      cout << "Could not insert labeled vertex into cluster map." << endl;
    }

    labeledVertices.push_back(make_pair(vertexid, label));
    
  }

  return labeledVertices;
}

int sign(const double num)
{
  return (0 < num) - (num < 0);
}

const DistancePair computeDistances(const Coordinates& point, const Experts& experts)
{
  Distances distances;

  distances.reserve(experts.size());

  double maxDistance = 0.0;

  for (const auto& expert : experts) {

    const double sqDistance = inner_product(point.begin(), point.end(),
      expert.mpCoordinates.begin(), 0.0,
      plus<double>(), [](const double a, const double b) {
        return (a - b) * (a - b);
      });

    distances.push_back(sqDistance);

    if (sqDistance > maxDistance) {
      maxDistance = sqDistance;
    }

  }

  return make_pair(distances, maxDistance);
}

Weights computeWeights(const Distances& distances, const MaxDistance maxDistance)
{
  Weights weights(distances.size());

  transform(distances.begin(), distances.end(),
    weights.begin(),
    [&maxDistance](double distance) {
      return exp(-distance / maxDistance);
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

const Weights computeNormalizedWeights(const Distances& distances, const MaxDistance maxDistance)
{
  Weights weights = computeWeights(distances, maxDistance);

  normalizeWeights(weights);

  return weights;
}

double computeDecisionSum(const Coordinates& point, const Experts& experts, const Weights& weights)
{
  vector<ExpertDecision> decisions(experts.size());

  transform(experts.begin(), experts.end(),
    weights.begin(), decisions.begin(),
    [&point](const Expert& expert, const double weight) {
      const double dotProduct = inner_product(point.begin(), point.end(),
        expert.differences.begin(), 0.0);
      return weight * (dotProduct - expert.bias);
    });

  return accumulate(decisions.begin(), decisions.end(), 0.0);
}

ClusterID labelVertex(const double decision_sum)
{
  return sign(decision_sum);
}

int insertLabeledVertexIntoClusterMap(ClusterMap& clusters, const VertexID vertexid, Vertex& vertex, const ClusterID label)
{
  if (clusters.find(label) == clusters.end()) {
    cout << "Error: Could not find cluster with label " << label << endl;
    return -1;
  }

  clusters[label].addVertex(vertexid, vertex);

  return 0;
}
