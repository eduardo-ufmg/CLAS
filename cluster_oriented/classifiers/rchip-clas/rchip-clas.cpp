#include <iostream>

#include "graphTypes.hpp"
#include "readFiles.hpp"
#include "label.hpp"
#include "writeFiles.hpp"
#include "stringHelpers.hpp"

using namespace std;

int main(int argc, char* argv[])
{
  if (argc < 4) {
    cerr << "Usage: " << argv[0] << " <dataset> <hyperplanes> <newVertices>" << endl;
    return 1;
  }

  const string& dataset_filename_with_path = argv[1];
  const string& hyperplanes_filename_with_path = argv[2];
  const string& new_vertices_filename_with_path = argv[3];

  ClusterMap clusters;

  if (readGraph(clusters, dataset_filename_with_path) != 0) {
    cerr << "Error reading graph from " << dataset_filename_with_path << endl;
    return 1;
  }

  Experts experts;

  if (readExperts(experts, hyperplanes_filename_with_path) != 0) {
    cerr << "Error reading hyperplanes from " << hyperplanes_filename_with_path << endl;
    return 1;
  }

  VertexMap newVertices;

  if (readVertices(newVertices, new_vertices_filename_with_path) != 0) {
    cerr << "Error reading new vertices from " << new_vertices_filename_with_path << endl;
    return 1;
  }

  const LabeledVertices labeledVertices = label(clusters, experts, newVertices);

  const string& aux_out_fn = outputPathFromInputPath(dataset_filename_with_path);
  const string& output_filename_with_path = aux_out_fn.substr(0, aux_out_fn.find_last_of('-')) + "-labeled.csv";

  if (writeLabeledVertices(labeledVertices, output_filename_with_path) != 0) {
    cerr << "Error writing output file" << output_filename_with_path << endl;
    return 1;
  } else {
    cout << "Labeled vertices written to " << output_filename_with_path << endl;
  }

  return 0;
}
