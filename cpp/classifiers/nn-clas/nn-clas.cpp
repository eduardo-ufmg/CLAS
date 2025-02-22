#include <iostream>

#include "graphTypes.hpp"
#include "readFiles.hpp"
#include "classify.hpp"
#include "writeFiles.hpp"
#include "stringHelpers.hpp"
#include "getSEs.hpp"

using namespace std;

int main(int argc, char* argv[])
{
  if (argc < 3) {
    cerr << "Usage: " << argv[0] << " <dataset> <new_vertices>" << endl;
    return 1;
  }

  string dataset_filename_with_path = argv[1];
  string new_vertices_filename_with_path = argv[2];

  ClusterMap clusters;

  if (readGraph(clusters, dataset_filename_with_path) != 0) {
    cerr << "Error reading dataset file " << dataset_filename_with_path << endl;
    return 1;
  }

  VertexMap new_vertices;

  if (readVertices(new_vertices, new_vertices_filename_with_path) != 0) {
    cerr << "Error reading new vertices file " << new_vertices_filename_with_path << endl;
    return 1;
  }

  NNsupportEdges nnSEs = getSEs(clusters);

  ClassifiedVertices classifiedVertices = classify(nnSEs, new_vertices, clusters);

  string aux_out_fn = outputPathFromInputPath(dataset_filename_with_path);
  string output_filename_with_path = aux_out_fn.substr(0, aux_out_fn.find_last_of('-')) + "-classified.csv";

  if (writeClassifiedVertices(classifiedVertices, output_filename_with_path) != 0) {
    cerr << "Error writing output file" << output_filename_with_path << endl;
    return 1;
  } else {
    cout << "Classified vertices written to " << output_filename_with_path << endl;
  }

  return 0;
}
