#include <iostream>

#include "types.hpp"
#include "filenameHelpers.hpp"
#include "readFiles.hpp"
#include "chipcid.hpp"
#include "gabrielGraph.hpp"
#include "filter.hpp"
#include "computeHyperplanes.hpp"
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

  const Hyperplanes hyperplanes = computeHyperplanes(vertices);

  const chipIDbimap chipidbimap = getchipIDmap(vertices, hyperplanes);

  const string output_file_path = "./train/chip-" + filenameFromPath(dataset_file_path);
  const string chipidmap_file_path = "./train/chipidbimap-" + filenameFromPath(dataset_file_path);

  if (writeHyperplanes(hyperplanes, output_file_path) != 0) {
    cerr << "Error: could not write hyperplanes to file" << output_file_path << endl;
    return 1;
  }

  if (writechipIDmap(chipidbimap, chipidmap_file_path) != 0) {
    cerr << "Error: could not write chipidbimap to file" << chipidmap_file_path << endl;
    return 1;
  }

}