#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cstdlib>

using namespace std;

std::string trim(const std::string &s) {
  size_t start = s.find_first_not_of(" \t");

  if(start == string::npos) {
    return "";
  }

  size_t end = s.find_last_not_of(" \t");
  return s.substr(start, end - start + 1);
}

std::string outputPathFromInputPath(const std::string &input_path) {
  string input_file_without_path = input_path.substr(input_path.find_last_of("/\\") + 1);
  string filtered_file_name = input_file_without_path.substr(0, input_file_without_path.find_last_of(".")) + "_filtered.csv";
  return "./output/" + filtered_file_name;
}

void buildGraph(vector< vector<int> > &adj, const vector< pair<int,int> > &edges, const size_t numVertices) {
  for(auto &e : edges) {
    int a = e.first, b = e.second;
    if(a >= 0 && a < static_cast<int>(numVertices) &&
       b >= 0 && b < static_cast<int>(numVertices)) {
      adj[a].push_back(b);
      adj[b].push_back(a);
    }
  }
}

void computeQ(vector<double> &q, const vector< vector<int> > &adj, const vector<int> &targets, const size_t numVertices) {
  for (size_t i = 0; i < numVertices; ++i) {
    int degree = adj[i].size();

    if(degree == 0) {
      q[i] = 0.0;
      continue;
    }

    int sameClassCount = 0;
    for (int nb : adj[i]) {
      if (targets[nb] == targets[i]) {
        sameClassCount++;
      }
    }

    q[i] = static_cast<double>(sameClassCount) / degree;
  }
}

void computeClassStats(map<int, pair<double, int>> &classStats, const vector<int> &targets, const vector<double> &q, const size_t numVertices) {
  for (size_t i = 0; i < numVertices; ++i) {
    int cls = targets[i];
    classStats[cls].first += q[i];
    classStats[cls].second += 1;
  }
}

void computeThresholds(map<int, double> &threshold, const map<int, pair<double, int>> &classStats) {
  for (auto &kv : classStats) {
    int cls = kv.first;
    double sum_q = kv.second.first;
    int count = kv.second.second;
    threshold[cls] = sum_q / count;
  }
}

void filterVertices(vector<bool> &keep, const vector<double> &q, const map<int, double> &threshold, const vector<int> &targets, const size_t numVertices) {
  for (size_t i = 0; i < numVertices; ++i) {
    int cls = targets[i];
    if (q[i] >= threshold.at(cls)) {
      keep[i] = true;
    }
    // else: vertex is filtered (removed)
  }
}

int reindexVertices(vector<int> &newIndex, const vector<bool> &keep, const size_t numVertices) {
  int newCounter = 0;
  for (size_t i = 0; i < numVertices; ++i) {
    if (keep[i]) {
      newIndex[i] = newCounter;
      newCounter++;
    }
  }
  return newCounter;
}

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

int main(int argc, char* argv[])
{
  if(argc < 4) {
    cerr << "Usage: " << argv[0] << " <features>.csv <targets>.csv <edges>.csv" << endl;
    return 1;
  }

  string features_file_name_with_path = argv[1];
  string targets_file_name_with_path = argv[2];
  string edges_file_name_with_path = argv[3];

  // -------------------------------
  // 1. Read vertices.csv
  // -------------------------------
  vector< vector<double> > vertices;
  if (readVertices(vertices, features_file_name_with_path) != 0) {
    return 1;
  }

  size_t numVertices = vertices.size();
  if (numVertices == 0) {
    cerr << "Error: No vertices were read." << endl;
    return 1;
  }

  // -------------------------------
  // 2. Read targets.csv
  // -------------------------------
  vector<int> targets;
  if (readTargets(targets, targets_file_name_with_path) != 0) {
    return 1;
  }

  if(targets.size() != numVertices) {
    cerr << "Error: Number of targets (" << targets.size() 
      << ") does not match number of vertices (" << numVertices << ")." << endl;
    return 1;
  }

  // -------------------------------
  // 3. Read edges.csv
  // -------------------------------
  vector< pair<int,int> > edges;
  if (readEdges(edges, edges_file_name_with_path) != 0) {
    return 1;
  }

  // -------------------------------
  // 4. Build the graph (adjacency list)
  // -------------------------------
  vector< vector<int> > adj(numVertices);
  buildGraph(adj, edges, numVertices);

  // -------------------------------
  // 5. Compute q(x_i) for each vertex
  // -------------------------------
  vector<double> q(numVertices, 0.0);
  computeQ(q, adj, targets, numVertices);

  // -------------------------------
  // 6. Compute the per-class threshold t_class
  // -------------------------------
  map<int, pair<double, int>> classStats;
  computeClassStats(classStats, targets, q, numVertices);

  map<int, double> threshold;
  computeThresholds(threshold, classStats);

  // -------------------------------
  // 7. Filter vertices based on q(x_i) vs. threshold
  // -------------------------------
  vector<bool> keep(numVertices, false);
  filterVertices(keep, q, threshold, targets, numVertices);

  // -------------------------------
  // 8. Re-index the remaining vertices
  // -------------------------------
  vector<int> newIndex(numVertices, -1);
  int newCounter = reindexVertices(newIndex, keep, numVertices);

  size_t numNewVertices = newCounter;
  cout << "Original vertices: " << numVertices << ", kept vertices: " << numNewVertices << "\n";

  // -------------------------------
  // 9. Write the filtered vertices and targets files
  // -------------------------------
  string filtered_features_file_name_with_path = outputPathFromInputPath(features_file_name_with_path);
  string filtered_targets_file_name_with_path = outputPathFromInputPath(targets_file_name_with_path);

  if (writeFilteredVertices(vertices, keep, filtered_features_file_name_with_path) != 0) {
    return 1;
  }

  if (writeFilteredTargets(targets, keep, filtered_targets_file_name_with_path) != 0) {
    return 1;
  }

  // -------------------------------
  // 10. Write the filtered edges file
  // -------------------------------
  string filtered_edges_file_name_with_path = outputPathFromInputPath(edges_file_name_with_path);

  if (writeFilteredEdges(edges, keep, newIndex, filtered_edges_file_name_with_path) != 0) {
    return 1;
  }

  cout << "Filtering complete. Files written:\n"
     << "  " << filtered_features_file_name_with_path << "\n"
     << "  " << filtered_targets_file_name_with_path << "\n"
     << "  " << filtered_edges_file_name_with_path << "\n";

  return 0;
}
