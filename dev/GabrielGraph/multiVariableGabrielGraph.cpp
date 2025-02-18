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
    cerr << "Error reading input file" << input_filename_with_path << endl;
    return 1;
  }
  
  gabrielGraph(clusters);

  string output_filename_with_path = outputPathFromInputPath(input_filename_with_path);

  if (writeGabrielGraphToFile(clusters, output_filename_with_path) != 0) {
    cerr << "Error writing output file" << output_filename_with_path << endl;
    return 1;
  }  else {
    cout << "Gabriel graph written to " << output_filename_with_path << endl;
  }

  return 0;
}
