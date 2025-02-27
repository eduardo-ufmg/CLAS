#include "readFiles.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#include "types.hpp"
#include "classifier.pb.h"

using namespace std;

ifstream openFile(const string& filename);
ClusterID parseCID(const classifierpb::ClusterID& cid);

ifstream openFile(const string& filename)
{
  ifstream file(filename);
  if (!file.is_open())    {
    cerr << "Error: could not open file " << filename << endl;
    exit(1);
  }
  return file;
}

Vertices readDataset(const string& filename)
{
  classifierpb::TrainingDataset dataset;
  
  ifstream file = openFile(filename);
  if (!dataset.ParseFromIstream(&file)) {
    cerr << "Error: could not parse file " << filename << endl;
    exit(1);
  }
  file.close();

  Vertices vertices;
  Clusters clusters;
  VertexID vcounter = 0;

  for (const auto& vertex : dataset.entries()) {
    const VertexID id = vcounter ++;
    const Coordinates coordinates(vertex.features().begin(), vertex.features().end());
    const ClusterID cid = parseCID(vertex.cluster_id());

    clusters.emplace(cid, Cluster(cid));

    vertices.emplace_back(id, coordinates, &clusters.at(cid));
  }

  return vertices;
}

ClusterID parseCID(const classifierpb::ClusterID& cid)
{
  switch(cid.cluster_id_case()) {
  case classifierpb::ClusterID::kClusterIdInt:
    return cid.cluster_id_int();
  case classifierpb::ClusterID::kClusterIdStr:
    return cid.cluster_id_str();
  default:
    cerr << "Error: unknown cluster id type" << endl;
    exit(1);
  }
}
