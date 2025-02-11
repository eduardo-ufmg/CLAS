#include "filter.hpp"

#include <vector>
#include <map>
#include <cmath>

using namespace std;

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
