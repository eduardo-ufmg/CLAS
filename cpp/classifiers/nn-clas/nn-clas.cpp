#include <iostream>

#include "graphTypes.hpp"
#include "readFiles.hpp"
#include "classify.hpp"
#include "writeFiles.hpp"
#include "stringHelpers.hpp"

using namespace std;

int main(int argc, char* argv[])
{
  if (argc < 4) {
    cerr << "Usage: " << argv[0] << " <dataset> <new_vertices>" << endl;
    return 1;
  }

  string dataset_filename_with_path = argv[1];
  string hyperplanes_filename_with_path = argv[2];
  string new_vertices_filename_with_path = argv[3];

  ClusterMap clusters;

  if (readGraph(clusters, dataset_filename_with_path) != 0) {
    cerr << "Error reading graph from " << dataset_filename_with_path << endl;
    return 1;
  }

  vector<Expert> experts;

  if (readExperts(experts, hyperplanes_filename_with_path) != 0) {
    cerr << "Error reading hyperplanes from " << hyperplanes_filename_with_path << endl;
    return 1;
  }

  VertexMap new_vertices;

  if (readVertices(new_vertices, new_vertices_filename_with_path) != 0) {
    cerr << "Error reading new vertices from " << new_vertices_filename_with_path << endl;
    return 1;
  }

  ClassifiedVertices classifiedVertices;

  classifiedVertices = classify(clusters, experts, new_vertices);

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
