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

int writeSVs(const SupportVertices& supportVertices, const std::string& filename)
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

int writeExperts(const Experts& experts, const std::string& filename)
{
  classifierpb::Experts pb_experts;

  for (const Expert& expert : experts) {
    classifierpb::ExpertEntry *pb_expert = pb_experts.add_entries();
    
    pb_expert->set_expert_id(expert.id);

    for (const float coord : expert.midpoint) {
      pb_expert->add_midpoint_coordinates(coord);
    }

    for (const float diff : expert.normal) {
      pb_expert->add_normal(diff);
    }

    pb_expert->set_bias(expert.bias);
  }

  ofstream file = openFileWrite(filename);
  if (!pb_experts.SerializeToOstream(&file)) {
    cerr << "Error: could not write experts to file" << filename << endl;
    return 1;
  }
  file.close();

  return 0;
}

int writeLabeledVertices(const LabeledVertices& labeledVertices, const std::string& filename)
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

int writechipIDmap(const chipIDbimap& chipidmap, const std::string& filename)
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
