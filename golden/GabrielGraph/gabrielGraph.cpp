#include "gabrielGraph.hpp"
#include <cmath>
#include <vector>

#include "squaredDistance.hpp"

using namespace std;

void gabrielGraph(vector<pair<size_t, size_t>>& edges, vector<vector<double>> points)
{
  size_t num_points;
  size_t num_coords;

  vector<double> midpoint;

  num_points = points.size();
  num_coords = points[0].size();

  edges.clear();
  midpoint.resize(num_coords);

  for (size_t i = 0; i < num_points; i++) {
    for (size_t j = i + 1; j < num_points; j++) {
      bool isGabrielEdge;
      double radius_sq;
      
      computeMidpoint(midpoint, points[i], points[j], num_coords);

      radius_sq = squaredDistance(points[i], midpoint);
      
      isGabrielEdge = true;

      checkGabrielEdge(isGabrielEdge, points, midpoint, radius_sq, num_points, i, j);

      if (isGabrielEdge) {
        edges.push_back({i, j});
      }

    }
  }

}

void computeMidpoint(vector<double>& midpoint, const vector<double>& a, const vector<double>& b, const size_t& num_coords)
{
  for (size_t i = 0; i < num_coords; i++) {
    midpoint[i] = (a[i] + b[i]) / 2;
  }
}

void checkGabrielEdge(bool& isGabrielEdge, const vector<vector<double>>& points, const vector<double>& midpoint, const double& radius_sq,
                      const size_t& num_points, const size_t& vertex1, const size_t& vertex2)
{

  for (size_t i = 0; i < num_points; i++) {
    if (i != vertex1 && i != vertex2) {
      double dist_sq;
      
      dist_sq = squaredDistance(points[i], midpoint);

      if (dist_sq < radius_sq) {
        isGabrielEdge = false;
        break;
      }

    }
  }
  
}
