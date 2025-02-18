#include <iostream>

#include "graphTypes.hpp"
#include "readGraph.hpp"
#include "computeExperts.hpp"
#include "printLoadedData.hpp"

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

  SupportEdges se = getSEs(clusters);

  vector<Expert> experts = getExperts(se, clusters);

  printExpertData(experts);

  return 0;
}
