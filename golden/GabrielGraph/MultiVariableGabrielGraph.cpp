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

  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " <input_csv>" << endl;
    return 1;
  }

  string input_filename = argv[1];
  ifstream infile(input_filename);

  if (!infile.is_open()) {
    cerr << "Error: Could not open file '" << input_filename << "'" << endl;
    return 1;
  }

  size_t num_cols = 0;

  vector<vector<double>> points;
  string line;

  while (getline(infile, line)) {
  
    if (line.empty()) {
      continue;
    }

    vector<double> point;
    stringstream ss(line);
    string token;

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

  size_t num_points = points.size();

  cout << "Number of instances (rows): " << num_points << endl;
  cout << "Number of features (columns): " << num_cols << endl;

  vector<pair<size_t, size_t>> edges;

  gabrielGraph(edges, points);

  string filename_without_slashes = input_filename.substr(input_filename.find_last_of("/\\") + 1);
  string output_filename = "./output/" + filename_without_slashes.substr(0, filename_without_slashes.find_last_of('-')) + "-edges.csv";
  ofstream outfile(output_filename);

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
