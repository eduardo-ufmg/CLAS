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
#include "readGraph.hpp"
#include "filter.hpp"
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

  string input_file_name_with_path = argv[1];

  ClusterMap clusters;

  if (readGraph(clusters, input_file_name_with_path) != 0) {
    return 1;
  }

  printClusterVertexQuantity(clusters);

  filterVertices(clusters, deviationFactor);

  printClusterStatistics(clusters);

  string aux_out_fn = outputPathFromInputPath(input_file_name_with_path);
  string output_file_name_with_path = aux_out_fn.substr(0, aux_out_fn.find_last_of('.')) + "-filtered.csv";

  if (writeGraphToFile(clusters, output_file_name_with_path) != 0) {
    return 1;
  }

}
