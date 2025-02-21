#include "classify.hpp"

#include <vector>
#include <map>
#include <cmath>
#include <iostream>
#include <numeric>
#include <algorithm>

#include "graphTypes.hpp"

using namespace std;

int sign(double num);
pair <vector<double>, double> computeDistances(const vector<double>& point, const vector<Expert>& experts);
vector<double> computeWeights(const vector<double>& distances, const double maxDistance);
vector<double> normalizeWeights(const vector<double>& weights);
vector<double> computeNormalizedWeights(const vector<double>& distances, const double maxDistance);
double computeDecisionSum(const vector<double>& point, const vector<Expert>& experts, const vector<double>& weights);
ClusterID classifyVertex(const double decision_sum);
int insertClassifiedVertexIntoClusterMap(ClusterMap& clusters, const VertexID vertexid, const shared_ptr<Vertex>& vertex, const ClusterID label);

ClassifiedVertices classify(ClusterMap& clusters, const vector<Expert>& experts, VertexMap& vertices)
{
  ClassifiedVertices classifiedVertices;
  
  for (auto& [vertexid, vertex] : vertices) {

    const vector<double>& point = vertex->features;

    // 1. Compute distances from point to each expert's midpoint
    auto [distances, maxDistance] = computeDistances(point, experts);

    // Prevent division by zero (if point equals all expert midpoints)
    if (maxDistance == 0.0) {
      maxDistance = 1e-8;
    }

    // 2. Compute expert weights using the gating function:
    //    c_l(point) = exp( - maxDistance^2 / D(point, pl) )
    vector<double> weights = computeNormalizedWeights(distances, maxDistance);

    // 3. Compute the weighted sum of expert decisions.
    // For each expert, h_l(point) = sign( dot(point, expert.differences) - expert.bias )
    double decision_sum = computeDecisionSum(point, experts, weights);

    // Final classification: f(point) = sign( sum_l c_l(point) * h_l(point) )
    ClusterID label = classifyVertex(decision_sum);

    // 4. Update the vertex's cluster assignment.
    if (insertClassifiedVertexIntoClusterMap(clusters, vertexid, vertex, label) != 0) {
      cout << "Could not insert classified vertex into cluster map." << endl;
    }

    classifiedVertices.push_back(make_pair(vertexid, label));
    
  }

  return classifiedVertices;
}

int sign(double num)
{
  return (0 < num) - (num < 0);
}

pair<vector<double>, double> computeDistances(const vector<double>& point, const vector<Expert>& experts)
{
  vector<double> distances;

  double maxDistance = 0.0;

  for (const Expert& expert : experts) {

    double sqDistance = inner_product(point.begin(), point.end(),
      expert.midpoint_coordinates.begin(), 0.0,
      plus<double>(), [](double a, double b) { return (a - b) * (a - b); });

    distances.push_back(sqDistance);

    if (sqDistance > maxDistance) {
      maxDistance = sqDistance;
    }

  }

  return make_pair(distances, maxDistance);
}

vector<double> computeWeights(const vector<double>& distances, const double maxDistance)
{
  vector<double> weights(distances.size());

  transform(distances.begin(), distances.end(),
    weights.begin(),
    [maxDistance](double distance) {
      return exp(-distance / maxDistance);
    });

  return weights;
}

vector<double> normalizeWeights(const vector<double>& weights)
{
  vector<double> normalizedWeights(weights.size());

  double sum = accumulate(weights.begin(), weights.end(), 0.0);

  if (sum == 0.0) {

    fill(normalizedWeights.begin(), normalizedWeights.end(), 1.0 / weights.size());

  } else {

    transform(weights.begin(), weights.end(),
      normalizedWeights.begin(),
      [sum](double weight) {
        return weight / sum;
      });

  }

  return normalizedWeights;
}

vector<double> computeNormalizedWeights(const vector<double>& distances, const double maxDistance)
{
  vector<double> weights = computeWeights(distances, maxDistance);

  weights = normalizeWeights(weights);

  return weights;
}

double computeDecisionSum(const vector<double>& point, const vector<Expert>& experts, const vector<double>& weights)
{
  vector<double> decisions(experts.size());

  transform(experts.begin(), experts.end(),
    weights.begin(), decisions.begin(),
    [point](const Expert& expert, double weight) {
      double dotProduct = inner_product(point.begin(), point.end(),
        expert.differences.begin(), 0.0);
      return weight * (dotProduct - expert.bias);
    });

  return accumulate(decisions.begin(), decisions.end(), 0.0);
}

ClusterID classifyVertex(const double decision_sum)
{
  return sign(decision_sum);
}

int insertClassifiedVertexIntoClusterMap(ClusterMap& clusters, const VertexID vertexid, const shared_ptr<Vertex>& vertex, const ClusterID label)
{
  if (clusters.find(label) == clusters.end()) {
    cout << "Error: Could not find cluster with label " << label << endl;
    return -1;
  }

  clusters[label].addVertex(vertexid, vertex);

  return 0;
}
