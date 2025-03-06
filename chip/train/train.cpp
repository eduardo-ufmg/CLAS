#include <iostream>

#include "types.hpp"
#include "filenameHelpers.hpp"
#include "readFiles.hpp"
#include "gabrielGraph.hpp"
#include "filter.hpp"
#include "computeExperts.hpp"
#include "writeFiles.hpp"

using namespace std;

int main(int argc, char** argv)
{

  float tolerance = ns_filter::DEFAULT_TOLERANCE;

  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " <dataset> [tolerance]" << endl;
  }

  if (argc > 2) {
    tolerance = stof(argv[2]);
  }

  const string dataset_file_path = argv[1];

  Vertices vertices = readDataset(dataset_file_path);

  computeGabrielGraph(vertices);

  filter(vertices, tolerance);

  const Experts experts = computeExperts(vertices);

  const string output_file_path = "./train/chips-" + filenameFromPath(dataset_file_path);

  if (writeExperts(experts, output_file_path) != 0) {
    cerr << "Error: could not write experts to file" << output_file_path << endl;
    return 1;
  }

}