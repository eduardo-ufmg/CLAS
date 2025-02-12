#include "buildGraph.hpp"

#include <vector>
#include <map>
#include <set>

#include "graphTypes.hpp"

using namespace std;

set<unsigned> getUniqueTargets(const vector<int>& raw_targets);

void buildVertexVector(vector<Vertex>& vertices, const vector< vector<double> >& raw_vertices, const vector< pair<int,int> >& raw_edges)
{
  unsigned num_vertices = raw_vertices.size();

  vertices.resize(num_vertices);

  for (unsigned i = 0; i < num_vertices; ++i) {
    Vertex v;
    v.features = raw_vertices[i]; // each raw_vertices is a vector features read from a line in the input file
    v.id = i;                     // id is the line number in the input file
    vertices[i] = v;
  }

  /*
    iterate over the raw_edges and fill the vertices adjacency lists
  */
  for (unsigned i = 0; i < raw_edges.size(); ++i) {
    vertices[raw_edges[i].first].adjacents.push_back(raw_edges[i].second);
    vertices[raw_edges[i].second].adjacents.push_back(raw_edges[i].first);
  }
}

void buildClusters(vector<Cluster>& clusters, const vector<Vertex>& all_vertices, const vector<int>& raw_targets)
{
  set<unsigned> unique_targets = getUniqueTargets(raw_targets);

  unsigned num_clusters = unique_targets.size();

  clusters.resize(num_clusters);

  for (auto& target : unique_targets) {
    clusters[target].id = target;
  }

  for (unsigned i = 0; i < all_vertices.size(); ++i) {
    clusters[raw_targets[i]].vertices.push_back(all_vertices[i]);
  }
}

set<unsigned> getUniqueTargets(const vector<int>& raw_targets)
{
  return set<unsigned>(raw_targets.begin(), raw_targets.end());
}

