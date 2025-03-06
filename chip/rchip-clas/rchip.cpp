#include "rchip.hpp"

#include <cmath>
#include <algorithm>
#include <numeric>

using namespace std;

int sign(const double num);
const Expert * const getClosestExpert(const Coordinates& point, const Experts& experts);
double computeHyperplaneSeparation(const Coordinates& point, const Expert& expert);
ClusterID labelVertex(const double separation);

const LabeledVertices rchip(const VerticesToLabel& vertices, const Experts& experts)
{

}