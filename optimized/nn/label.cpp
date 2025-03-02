#include <iostream>

#include "types.hpp"
#include "readFiles.hpp"
#include "writeFiles.hpp"

using namespace std;

int main(int argc, char **argv)
{
  if (argc < 3) {
    cerr << "Usage: " << argv[0] << " <tolabel> <support_vertices>" << endl;
    return 1;
  }

  const string tolabel_file_path = argv[1];
  const string support_vertices_file_path = argv[2];

  Vertices toLabel = readToLabel(tolabel_file_path);
  SupportVertices supportVertices = readSVs(support_vertices_file_path);

  return 0;
}