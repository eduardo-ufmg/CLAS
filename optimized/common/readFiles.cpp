#include "readFiles.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "types.hpp"
#include "classifier.pb.h"

using namespace std;

ifstream openFile(const string& filename);
ClusterID parseCID(const classifierpb::ClusterID& cid);

Vertices readDataset(const string& filename)
{
  classifierpb::TrainingDataset dataset;
  
  ifstream file = openFile(filename);

  if (!dataset.ParseFromIstream(&file)) {

    #if DEBUG
    cout << "DEBUG_START: DATASET PARSE ERROR" << endl;
    dataset.PrintDebugString();
    cout << "DEBUG_END: DATASET PARSE ERROR" << endl;
    #endif

    throw runtime_error("Error: could not parse dataset");
  }

  file.close();

  Vertices vertices;
  Clusters clusters;
  VertexID vcounter = 0;

  #if DEBUG
  cout << "DEBUG_START: PRINT PARSED DATASET" << endl;
  dataset.PrintDebugString();
  cout << "DEBUG_END: PRINT PARSED DATASET" << endl;
  #endif

  #if DEBUG
  int debug_counter = 0;
  #endif

  for (const auto& vertex : dataset.entries()) {

    #if DEBUG
    cout << "DEBUG_START: VERTEX " << debug_counter << endl;
    vertex.PrintDebugString();
    cout << "DEBUG_END: VERTEX " << debug_counter ++ << endl;
    #endif

    const VertexID id = vcounter ++;
    const Coordinates coordinates(vertex.features().begin(), vertex.features().end());
    const ClusterID cid = parseCID(vertex.cluster_id());

    clusters.emplace(cid, make_shared<Cluster>(cid));

    vertices.emplace_back(id, coordinates, clusters.at(cid));

    #if DEBUG
    cout << "DEBUG: VERTEX PARSED" << endl;
    #endif
  }

  #if DEBUG
  cout << "DEBUG: ALL VERTICES PARSED" << endl;
  #endif

  return vertices;
}

ifstream openFile(const string& filename)
{
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ifstream file(filename, ios::binary);
  if (!file.is_open())    {
    throw runtime_error("Error: could not open file");
  }
  return file;
}

ClusterID parseCID(const classifierpb::ClusterID& cid)
{
  if (!cid.has_cluster_id_int() && !cid.has_cluster_id_str()) {
    throw runtime_error("Error: cluster id did not have any value");
  }

  switch(cid.cluster_id_case()) {
  case classifierpb::ClusterID::kClusterIdInt:
    return cid.cluster_id_int();
  case classifierpb::ClusterID::kClusterIdStr:
    return cid.cluster_id_str();
  default:
    throw runtime_error("Error: cluster id did not match any case");
  }
}
