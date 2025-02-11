#include "writeFiles.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int writeFilteredVertices(const vector< vector<double> > &vertices, const vector<bool> &keep, const string &filtered_features_file_name_with_path) {
  ofstream fout(filtered_features_file_name_with_path);
  if (!fout) {
    cerr << "Error: Cannot open output vertices file for writing." << endl;
    return 1;
  }

  for (size_t i = 0; i < vertices.size(); ++i) {
    if (!keep[i]) {
      continue;
    }

    // Write coordinates as comma–separated values.
    for (size_t j = 0; j < vertices[i].size(); ++j) {
      fout << vertices[i][j];
      if (j < vertices[i].size()-1) {
        fout << ", ";
      }
    }

    fout << "\n";
  }

  fout.close();
  return 0;
}

int writeFilteredTargets(const vector<int> &targets, const vector<bool> &keep, const string &filtered_targets_file_name_with_path) {
  ofstream fout(filtered_targets_file_name_with_path);
  if (!fout) {
    cerr << "Error: Cannot open output targets file for writing." << endl;
    return 1;
  }

  for (size_t i = 0; i < targets.size(); ++i) {
    if (!keep[i]) {
      continue;
    }

    fout << targets[i] << "\n";
  }

  fout.close();
  return 0;
}

int writeFilteredEdges(const vector< pair<int,int> > &edges, const vector<bool> &keep, const vector<int> &newIndex, const string &filtered_edges_file_name_with_path) {
  ofstream fout(filtered_edges_file_name_with_path);
  if (!fout) {
    cerr << "Error: Cannot open output edges file for writing." << endl;
    return 1;
  }

  for (auto &e : edges) {
    int a = e.first;
    int b = e.second;
    if (keep[a] && keep[b]) {
      fout << newIndex[a] << ", " << newIndex[b] << "\n";
    }
  }

  fout.close();
  return 0;
}
