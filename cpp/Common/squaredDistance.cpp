#include "squaredDistance.hpp"

#include <limits>
#include <numeric>
#include <vector>

using namespace std;

double squaredDistance(const vector<double>& a, const vector<double>& b)
{
  if (a.size() != b.size()) {
    return numeric_limits<double>::infinity();
   }
 
   return inner_product(a.begin(), a.end(),
     b.begin(), 0.0, plus<double>(),
     [](double x, double y) {
       return (x - y) * (x - y);
     });
}