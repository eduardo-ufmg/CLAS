#include "writeFiles.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "classifier.pb.h"
#include "types.hpp"

using namespace std;

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

ofstream openFileWrite(const string& filename);

int writeSVs(const Vertices& supportVertices, const std::string& filename)
{
  classifierpb::SupportVertices pb_supportVertices;

  for (const Vertex& vertex : supportVertices) {
    classifierpb::SupportVertexEntry *pb_vertex = pb_supportVertices.add_entries();
    
    pb_vertex->set_vertex_id(vertex.id);

    for (const float coord : vertex.coordinates) {
      pb_vertex->add_features(coord);
    }

    classifierpb::ClusterID pb_clusterid;

    visit(overloaded {
      [&pb_clusterid](const int id) { pb_clusterid.set_cluster_id_int(id); },
      [&pb_clusterid](const string& id) { pb_clusterid.set_cluster_id_str(id); }
    }, vertex.cluster->id);


    pb_vertex->set_allocated_cluster_id(&pb_clusterid);
  }

  ofstream file = openFileWrite(filename);
  if (!pb_supportVertices.SerializeToOstream(&file)) {
    cerr << "Error: could not write SVs to file" << filename << endl;
    return 1;
  }
  file.close();

  return 0;
}


ofstream openFileWrite(const string& filename)
{
  ofstream file(filename, ios::binary);
  if (!file.is_open()) {
    throw runtime_error("Could not open file " + filename);
  }
  return file;
}
