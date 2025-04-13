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
    cerr << "Usage: " << argv[0] << " <tolabel> <hyperplanes>" << endl;
    return 1;
  }

  const string tolabel_path = argv[1];
  const string hyperplanes_path = argv[2];

  const string hyperplanes_name = filenameFromPath(hyperplanes_path);
  const string chipidbimap_path = parentFolder(hyperplanes_path) + "/chipidbimap-" + datasetFromFilename(hyperplanes_name);

  const VerticesToLabel verticestl = readToLabel(tolabel_path);
  const Hyperplanes hyperplanes = readHyperplanes(hyperplanes_path);
  const chipIDbimap chipidbimap = readchipIDmap(chipidbimap_path);

  const LabeledVertices labeledVertices = chip(verticestl, hyperplanes, chipidbimap);

  const string dataset_name = hyperplanes_name.substr(hyperplanes_name.find("-") + 1);
  const string labeled_vertices_path = "./label/chip-" + dataset_name;

  if (writeLabeledVertices(labeledVertices, labeled_vertices_path) != 0) {
    cerr << "Error: could not write labeled vertices" << endl;
    return 1;
  }
}