#include"squaredDistance.hpp"
#include <vector>
#include <cmath>

using namespace std;

double squaredDistance(const vector<double>& a, const vector<double>& b)
{
  double dist2 = 0.0;

  for (size_t d = 0; d < a.size(); d++) {
    double diff = a[d] - b[d];
    dist2 += diff * diff;
  }

  return dist2;
}
