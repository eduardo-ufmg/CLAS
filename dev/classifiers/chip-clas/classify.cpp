#include "classify.hpp"

#include <vector>
#include <map>
#include <cmath>
#include <iostream>

#include "graphTypes.hpp"

using namespace std;

int sign(double num);

ClassifiedVertices classify(ClusterMap& clusters, const vector<Expert>& experts, VertexMap& vertices)
{
  ClassifiedVertices classifiedVertices;
  // Iterate over all new vertices
  for (auto& [vertex_id, vertex_ptr] : vertices) {
    const vector<double>& x = vertex_ptr->features;
    size_t dim = x.size();

    // 1. Compute distances from x to each expert's midpoint
    vector<double> distances;
    distances.reserve(experts.size());
    double sigma = 0.0; // will be max distance
    for (const Expert& expert : experts) {
      double d = 0.0;
      // Compute Euclidean distance
      for (size_t i = 0; i < dim; ++i) {
        double diff = x[i] - expert.midpoint_coordinates[i];
        d += diff * diff;
      }
      d = sqrt(d);
      distances.push_back(d);
      if (d > sigma) {
        sigma = d;
      }
    }
    // Prevent division by zero (if x equals all expert midpoints)
    if (sigma == 0.0)
      sigma = 1e-8;

    // 2. Compute expert weights using the gating function:
    //    c_l(x) = exp( - sigma^2 / D(x, pl) )
    vector<double> weights;
    weights.reserve(experts.size());
    double sum_weights = 0.0;
    for (double d : distances) {
      double w = exp( - (sigma * sigma) / d );
      weights.push_back(w);
      sum_weights += w;
    }
    // Normalize weights
    if (sum_weights == 0.0) {
      // If all weights are zero (should not normally happen), use uniform weights.
      for (double &w : weights) {
        w = 1.0 / experts.size();
      }
    } else {
      for (double &w : weights) {
        w /= sum_weights;
      }
    }

    // 3. Compute the weighted sum of expert decisions.
    // For each expert, h_l(x) = sign( dot(x, expert.differences) - expert.bias )
    double decision_sum = 0.0;
    for (size_t i = 0; i < experts.size(); ++i) {
      const Expert& expert = experts[i];
      double dot_product = 0.0;
      for (size_t j = 0; j < dim; ++j)
        dot_product += x[j] * expert.differences[j];
      double activation = dot_product - expert.bias;
      int h = sign(activation);
      decision_sum += weights[i] * h;
    }

    // Final classification: f(x) = sign( sum_l c_l(x) * h_l(x) )
    int final_label = sign(decision_sum);

    // 4. Update the vertex's cluster assignment.
    // We use the final_label (either +1 or -1) as the key in the clusters map.
    ClassType cluster_key = final_label;
    if (clusters.find(cluster_key) == clusters.end()) {
      // Create a new cluster if one does not exist for this label.
      clusters[cluster_key] = Cluster();
    }
    Cluster* assigned_cluster = &clusters[cluster_key];
    vertex_ptr->cluster = assigned_cluster;
    assigned_cluster->vertices[vertex_id] = vertex_ptr;

    classifiedVertices.push_back(make_pair(vertex_id, final_label));
  }

  return classifiedVertices;
}

int sign(double num)
{
  return (0 < num) - (num < 0);
}
