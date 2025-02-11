#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>

#include "squaredDistance.hpp"
#include "gabrielGraph.hpp"

using namespace std;

int main(int argc, char* argv[])
{
  string input_filename;
  ifstream infile;

  vector<vector<double>> points;
  vector<pair<size_t, size_t>> edges;

  string line;
  size_t num_cols;

  size_t num_points;

  string filename_without_slashes;
  string output_filename;
  ofstream outfile;

  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " <input_csv>" << endl;
    return 1;
  }

  input_filename = argv[1];
  infile.open(input_filename);

  if (!infile.is_open()) {
    cerr << "Error: Could not open file '" << input_filename << "'" << endl;
    return 1;
  }

  num_cols = 0;

  while (getline(infile, line)) {
    vector<double> point;
    stringstream ss;
    string token;

    ss.str(line);

    if (line.empty()) {
      continue;
    }

    while (getline(ss, token, ',')) {
      try {
        point.push_back(stod(token));
      } catch (const std::exception& e) {
        cerr << "Error converting token '" << token << "' to a number." << endl;
        return 1;
      }
    }

    if (num_cols == 0) {
      num_cols = point.size();
    } else if (point.size() != num_cols) {
      cerr << "Error: Inconsistent number of columns in the input file." << endl;
      return 1;
    }

    points.push_back(point);
  }

  infile.close();

  num_points = points.size();

  cout << "Number of instances (rows): " << num_points << endl;
  cout << "Number of features (columns): " << num_cols << endl;

  gabrielGraph(edges, points);

  filename_without_slashes = input_filename.substr(input_filename.find_last_of("/\\") + 1);
  output_filename = "./output/" + filename_without_slashes.substr(0, filename_without_slashes.find_last_of('-')) + "-edges.csv";
  outfile.open(output_filename);

  if (!outfile.is_open()){
    cerr << "Error: Could not open output file " << output_filename << endl;
    return 1;
  }

  for (const auto& edge : edges) {
    outfile << edge.first << "," << edge.second << "\n";
  }
  
  outfile.close();

  cout << "Gabriel Graph computed. Number of edges: " << edges.size() << endl;
  cout << "Edges have been saved in " << output_filename << endl;

  return 0;
}
