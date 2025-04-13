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

int writeSVs(const SupportVertices& supportVertices, const string& filename)
{
  classifierpb::SupportVertices pb_supportVertices;

  for (const SupportVertex& vertex : supportVertices) {
    classifierpb::SupportVertexEntry *pb_vertex = pb_supportVertices.add_entries();
    
    pb_vertex->set_vertex_id(vertex.id);

    for (const float coord : vertex.coordinates) {
      pb_vertex->add_features(coord);
    }

    classifierpb::ClusterID * pb_clusterid = make_unique<classifierpb::ClusterID>().release();

    visit(overloaded {
      [pb_clusterid](const int id) { pb_clusterid->set_cluster_id_int(id); },
      [pb_clusterid](const string& id) { pb_clusterid->set_cluster_id_str(id); }
    }, vertex.clusterid);


    pb_vertex->set_allocated_cluster_id(pb_clusterid);
  }

  ofstream file = openFileWrite(filename);
  if (!pb_supportVertices.SerializeToOstream(&file)) {
    cerr << "Error: could not write SVs to file" << filename << endl;
    return 1;
  }
  file.close();

  return 0;
}

int writeHyperplanes(const Hyperplanes& hyperplanes, const string& filename)
{
  classifierpb::Hyperplanes pb_hyperplanes;

  for (const Hyperplane& hyperplane : hyperplanes) {
    classifierpb::HyperplaneEntry *pb_hyperplane = pb_hyperplanes.add_entries();
    
    pb_hyperplane->set_hyperplane_id(hyperplane.id);

    for (const float coord : hyperplane.edgeMidpoint) {
      pb_hyperplane->add_edge_midpoint_coordinates(coord);
    }

    for (const float diff : hyperplane.normal) {
      pb_hyperplane->add_normal(diff);
    }

    pb_hyperplane->set_bias(hyperplane.bias);
  }

  ofstream file = openFileWrite(filename);
  if (!pb_hyperplanes.SerializeToOstream(&file)) {
    cerr << "Error: could not write hyperplanes to file" << filename << endl;
    return 1;
  }
  file.close();

  return 0;
}

int writeLabeledVertices(const LabeledVertices& labeledVertices, const string& filename)
{
  classifierpb::LabeledVertices pb_labeledVertices;

  for (const LabeledVertex& vertex : labeledVertices) {
    classifierpb::LabeledVertexEntry *pb_vertex = pb_labeledVertices.add_entries();
    
    pb_vertex->set_vertex_id(vertex.id);

    for (const float coord : vertex.coordinates) {
      pb_vertex->add_features(coord);
    }

    classifierpb::ClusterID * pb_clusterid = make_unique<classifierpb::ClusterID>().release();

    visit(overloaded {
      [pb_clusterid](const int id) { pb_clusterid->set_cluster_id_int(id); },
      [pb_clusterid](const string& id) { pb_clusterid->set_cluster_id_str(id); }
    }, vertex.clusterid);

    pb_vertex->set_allocated_cluster_id(pb_clusterid);
  }

  ofstream file = openFileWrite(filename);
  if (!pb_labeledVertices.SerializeToOstream(&file)) {
    cerr << "Error: could not write labeled vertices to file" << filename << endl;
    return 1;
  }
  file.close();

  return 0;
}

int writechipIDmap(const chipIDbimap& chipidmap, const string& filename)
{
  classifierpb::chipIDmap pb_chipidmap;

  for (const auto& [chip, cid] : chipidmap.getchiptocid()) {
    classifierpb::chipIDpair *pb_pair = pb_chipidmap.add_entries();
    
    pb_pair->set_chip_int(chip);
    
    classifierpb::ClusterID * pb_clusterid = make_unique<classifierpb::ClusterID>().release();

    visit(overloaded {
      [pb_clusterid](const int id) { pb_clusterid->set_cluster_id_int(id); },
      [pb_clusterid](const string& id) { pb_clusterid->set_cluster_id_str(id); }
    }, cid);

    pb_pair->set_allocated_cluster_id(pb_clusterid);

  }

  ofstream file = openFileWrite(filename);
  if (!pb_chipidmap.SerializeToOstream(&file)) {
    cerr << "Error: could not write chipIDmap to file" << filename << endl;
    return 1;
  }
  file.close();

  return 0;
}

ofstream openFileWrite(const string& filename)
{
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ofstream file(filename, ios::binary);
  if (!file.is_open()) {
    throw runtime_error("Could not open file " + filename);
  }
  return file;
}
