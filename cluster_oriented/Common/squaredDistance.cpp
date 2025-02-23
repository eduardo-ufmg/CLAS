#include "squaredDistance.hpp"

#include <limits>
#include <numeric>
#include <vector>

using namespace std;

double squaredDistance(const Coordinates& a, const Coordinates& b)
{
  if (a.size() != b.size()) {
    return numeric_limits<double>::infinity();
  }
 
  return inner_product(a.begin(), a.end(),
    b.begin(), 0.0, plus<double>(),
    [](const double x, const double y) {
      return (x - y) * (x - y);
    });
}