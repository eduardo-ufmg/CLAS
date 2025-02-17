#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>
#include <variant>

#include "graphTypes.hpp"
#include "stringHelpers.hpp"
#include "printLoadedData.hpp"
#include "readGraph.hpp"
#include "gabrielGraph.hpp"
#include "writeFiles.hpp"

using namespace std;

int main(int argc, char* argv[])
{
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " <input_csv>" << endl;
    return 1;
  }

  string input_filename_with_path = argv[1];

  ClusterMap clusters;

  if (readGraph(clusters, input_filename_with_path) != 0) {
    return 1;
  }

  printClusterVertexQuantity(clusters);
  
  gabrielGraph(clusters);

  printAdjacencyLists(clusters, 4, 4);

  string output_filename_with_path = outputPathFromInputPath(input_filename_with_path);

  if (writeGraphToFile(clusters, output_filename_with_path) != 0) {
    return 1;
  }

  return 0;
}
