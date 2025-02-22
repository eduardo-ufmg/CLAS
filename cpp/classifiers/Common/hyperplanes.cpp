#include <iostream>

#include "graphTypes.hpp"
#include "readFiles.hpp"
#include "computeExperts.hpp"
#include "printLoadedData.hpp"
#include "writeFiles.hpp"
#include "stringHelpers.hpp"

using namespace std;

int main(int argc, char* argv[])
{
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " <input_csv>" << endl;
    return 1;
  }

  const string input_filename_with_path = argv[1];

  ClusterMap clusters;

  if (readGraph(clusters, input_filename_with_path) != 0) {
    cerr << "Error reading graph from " << input_filename_with_path << endl;
    return 1;
  }

  const auto& se = getSEs(clusters);

  const vector<Expert> experts = getExperts(se, clusters);

  const string output_filename_with_path = outputPathFromInputPath(input_filename_with_path);

  if (writeExpertsToFile(experts, output_filename_with_path) != 0) {
    cerr << "Error writing experts to " << output_filename_with_path << endl;
    return 1;
  }

  return 0;
}
