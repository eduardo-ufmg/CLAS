#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>

using namespace std;

// Helper function: computes the squared Euclidean distance between two points.
double squaredDistance(const vector<double>& a, const vector<double>& b)
{
  double dist2 = 0.0;

  for (size_t d = 0; d < a.size(); d++) {
    double diff = a[d] - b[d];
    dist2 += diff * diff;
  }

  return dist2;
}

int main(int argc, char* argv[])
{
  // Check for the proper number of command-line arguments.
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " <input_csv>" << endl;
    return 1;
  }
  
  // Open the input CSV file.
  string input_filename(argv[1]);
  ifstream infile(input_filename);

  if (!infile.is_open()) {
    cerr << "Error: Could not open file '" << input_filename << "'" << endl;
    return 1;
  }

  // Read the points from the CSV file.
  // Each row is expected to be a comma-separated list of numbers.
  vector<vector<double>> points;
  string line;
  size_t num_cols = 0;

  while (getline(infile, line)) {
    vector<double> point;
    stringstream ss(line);
    string token;

    if (line.empty()) {
      continue; // skip empty lines
    }

    while (getline(ss, token, ',')) {
      try {
        point.push_back(stod(token));
      } catch (const std::exception& e) {
        cerr << "Error converting token '" << token << "' to a number." << endl;
        return 1;
      }
    }

    // Set the number of columns based on the first row.
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

  // Compute the Gabriel Graph.
  // An edge between points i and j is included if and only if
  // the (hyper)disk with the segment [i, j] as diameter does NOT contain any other point in its interior.
  vector<pair<int, int>> edges;

  // Iterate over all unique pairs of points.
  for (size_t i = 0; i < num_points; i++) {
    for (size_t j = i + 1; j < num_points; j++) {
      // Compute the midpoint of points[i] and points[j].
      vector<double> midpoint(num_cols, 0.0);
      for (size_t d = 0; d < num_cols; d++) {
        midpoint[d] = (points[i][d] + points[j][d]) / 2.0;
      }
      // The squared radius is the squared distance from the midpoint to either endpoint.
      double radius_sq = squaredDistance(points[i], midpoint);
      
      // Check all other points to ensure none lies strictly inside the disk.
      bool isGabrielEdge = true;
      for (size_t k = 0; k < num_points; k++) {
        if (k == i || k == j)
          continue;
        double dist_sq = squaredDistance(points[k], midpoint);
        // If a point lies strictly inside the disk, the edge is not part of the Gabriel Graph.
        // (Points exactly on the circle are allowed.)
        if (dist_sq < radius_sq) {
          isGabrielEdge = false;
          break;
        }
      }
      if (isGabrielEdge) {
        edges.push_back({static_cast<int>(i), static_cast<int>(j)});
      }
    }
  }

  // Write the edges to a CSV file.
  // Each line contains two integers: the indices of the connected points.
  string filename = input_filename.substr(input_filename.find_last_of("/\\") + 1);
  string output_filename = "./output/" + filename.substr(0, filename.find_last_of('.')) + "_edges.csv";
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
