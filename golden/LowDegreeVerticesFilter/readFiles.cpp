#include "readFiles.hpp"

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>

#include "stringHelpers.hpp"

using namespace std;

int readVertices(vector< vector<double> > &vertices, const string &features_file_name_with_path) {
  ifstream fin(features_file_name_with_path);
  if (!fin) {
    cerr << "Error: Cannot open vertices file: " << features_file_name_with_path << endl;
    return 1;
  }

  string line;
  while(getline(fin, line)) {

    if(line.empty()) {
      continue;
    }

    vector<double> coords;
    stringstream ss(line);
    string token;

    while(getline(ss, token, ',')) {
      token = trim(token);

      if (!token.empty()) {
        coords.push_back(stod(token));
      }
    }

    vertices.push_back(coords);
  }

  return 0;
}

int readTargets(vector<int> &targets, const string &targets_file_name_with_path) {
  ifstream fin(targets_file_name_with_path);
  if (!fin) {
    cerr << "Error: Cannot open targets file: " << targets_file_name_with_path << endl;
    return 1;
  }

  string line;
  while(getline(fin, line)) {
    line = trim(line);

    if(line.empty()) {
      continue;
    }

    targets.push_back(stoi(line));
  }

  return 0;
}

int readEdges(vector< pair<int,int> > &edges, const string &edges_file_name_with_path) {
  ifstream fin(edges_file_name_with_path);
  if (!fin) {
    cerr << "Error: Cannot open edges file: " << edges_file_name_with_path << endl;
    return 1;
  }

  string line;
  while(getline(fin, line)) {

    if(line.empty()) {
      continue;
    }

    stringstream ss(line);
    string token;
    int a, b;

    if(getline(ss, token, ',')) {
      token = trim(token);
      a = stoi(token);
    } else {
      continue;
    }

    if(getline(ss, token, ',')) {
      token = trim(token);
      b = stoi(token);
    } else {
      continue;
    }

    edges.push_back(make_pair(a, b));
  }

  return 0;
}
