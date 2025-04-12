#include <iostream>

#include "types.hpp"

using namespace std;

int main()
{
  // Create some vertices with coordinates that result in different normal and bias
  Vertex v1(1, {2.0f, 1.0f});
  Vertex v2(2, {1.0f, 2.0f});

  // Create an edge
  Edge edge(&v1, &v2);

  // Create an ExpertCHIP instance
  ExpertCHIP chipExpert(1, edge);

  // Create an ExpertRCHIP instance
  ExpertRCHIP rchipExpert(2, edge);

  // Create an ExpertPred instance
  Coordinates midpoint = BaseExpert::computeMidpoint(edge);
  NormalVector normal = {0.5f, 0.5f};
  float bias = 1.0f;
  ExpertPred predExpert(3, midpoint, normal, bias);

  // Example usage
  cout << "CHIP Expert ID: " << chipExpert.id << endl;
  cout << "Midpoint: (" << chipExpert.midpoint[0] << ", " << chipExpert.midpoint[1] << ")" << endl;
  cout << "Normal: (" << chipExpert.normal[0] << ", " << chipExpert.normal[1] << ")" << endl;
  cout << "Bias: " << chipExpert.bias << endl;

  cout << "RCHIP Expert ID: " << rchipExpert.id << endl;
  cout << "Midpoint: (" << rchipExpert.midpoint[0] << ", " << rchipExpert.midpoint[1] << ")" << endl;
  cout << "Normal: (" << rchipExpert.normal[0] << ", " << rchipExpert.normal[1] << ")" << endl;
  cout << "Bias: " << rchipExpert.bias << endl;

  cout << "Pred Expert ID: " << predExpert.id << endl;
  cout << "Midpoint: (" << predExpert.midpoint[0] << ", " << predExpert.midpoint[1] << ")" << endl;
  cout << "Normal: (" << predExpert.normal[0] << ", " << predExpert.normal[1] << ")" << endl;
  cout << "Bias: " << predExpert.bias << endl;

  return 0;
}
