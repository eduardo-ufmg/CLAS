#include <iostream>

#include "types.hpp"
#include "filenameHelpers.hpp"
#include "readFiles.hpp"
#include "chip.hpp"
#include "writeFiles.hpp"

using namespace std;

int main(int argc, char **argv)
{
  if (argc < 3) {
    cerr << "Usage: " << argv[0] << " <tolabel> <experts>" << endl;
    return 1;
  }

  const string tolabel_path = argv[1];
  const string experts_path = argv[2];

  const VerticesToLabel verticestl = readToLabel(tolabel_path);
  const Experts experts = readExperts(experts_path);

  const LabeledVertices labeledVertices = chip(verticestl, experts);

  const string experts_name = filenameFromPath(experts_path);
  const string dataset_name = experts_name.substr(experts_name.find("-") + 1);
  const string labeled_vertices_path = "./label/chip-" + dataset_name;

  if (writeLabeledVertices(labeledVertices, labeled_vertices_path) != 0) {
    cerr << "Error: could not write labeled vertices" << endl;
    return 1;
  }
}