#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cstdlib>

#include "graphTypes.hpp"
#include "stringHelpers.hpp"
#include "printLoadedData.hpp"
#include "readFiles.hpp"
#include "computeFilter.hpp"
#include "writeFiles.hpp"

using namespace std;

const double DEFAULT_DEVIATION_FACTOR = 1.0;

int main(int argc, char* argv[])
{
  if(argc < 2) {
    cerr << "Usage: " << argv[0] << " <graph>.csv [deviation_factor]" << endl;
    return 1;
  }

  double deviationFactor = DEFAULT_DEVIATION_FACTOR;

  if(argc > 2) {
    deviationFactor = atof(argv[2]);
  }

  string input_filename_with_path = argv[1];

  ClusterMap clusters;

  if (readGraph(clusters, input_filename_with_path) != 0) {
    cerr << "Error reading input file" << input_filename_with_path << endl;
    return 1;
  }

  filterVertices(clusters, deviationFactor);

  string aux_out_fn = outputPathFromInputPath(input_filename_with_path);
  string output_filename_with_path = aux_out_fn.substr(0, aux_out_fn.find_last_of('.')) + "-filtered.csv";

  if (writeVerticesToFile(clusters, output_filename_with_path) != 0) {
    cerr << "Error writing output file" << output_filename_with_path << endl;
    return 1;
  } else {
    cout << "Filtered vertices written to " << output_filename_with_path << endl;
  }

}
