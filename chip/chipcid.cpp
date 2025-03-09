#include "chipcid.hpp"

#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <iostream>

#include "squaredDistance.hpp"

using namespace std;

template <typename VT>
const VT getuptoNVerticesforeachLabel(const VT& vertices, const size_t n);
const VerticesToLabel vtlfromVertices(const Vertices& vertices);
const LabeledVertices auxrchip(const VerticesToLabel& vertices, const Experts& experts);

const chipIDbimap getchipIDmap(const Vertices& vertices, const Experts& experts)
{
#define HACKY_GETCHIPIDMAP

#ifdef HACKY_GETCHIPIDMAP

  using RefClusterID = ClusterID;
  using LbdClusterID = int;
  using RefvsLbdVector = vector< pair<RefClusterID, LbdClusterID> >;
  using IDCounter = vector< pair<ClusterID, size_t> >;
  using RefLbdCounter = map<RefClusterID, IDCounter>;

  const VerticesToLabel refVertices = vtlfromVertices(getuptoNVerticesforeachLabel(vertices, 16));
  const LabeledVertices lbdVertices = auxrchip(refVertices, experts);

  RefvsLbdVector refVSlbd;
  refVSlbd.reserve(refVertices.size());

  transform(refVertices.begin(), refVertices.end(),
            lbdVertices.begin(),
            back_inserter(refVSlbd),
            [](const VertexToLabel& ref, const LabeledVertex& lbd) {
              return make_pair(ref.expectedclusterid, get<int>(lbd.clusterid));
            });

  RefLbdCounter refLbdCounter;

  for (const auto& refVSlbdpair : refVSlbd) {
    
    const RefClusterID& refClusterID = refVSlbdpair.first;
    const LbdClusterID& lbdClusterID = refVSlbdpair.second;

    if (refLbdCounter.find(refClusterID) == refLbdCounter.end()) {
      refLbdCounter.emplace(refClusterID, IDCounter({{lbdClusterID, 1}}));
    } else {

      for (auto& idcounter : refLbdCounter[refClusterID]) {
        if (get<int>(idcounter.first) == lbdClusterID) {
          idcounter.second ++;
          goto found;
        }
      }

      refLbdCounter[refClusterID].push_back({lbdClusterID, 1});

      found: ;
    }

  }

  chipIDbimap chipidbimap;

  for (auto& refLbdCounterPair : refLbdCounter) {
    
    const RefClusterID& refClusterID = refLbdCounterPair.first;
    const IDCounter& idCounter = refLbdCounterPair.second;

    const auto maxIDCounter = max_element(idCounter.begin(), idCounter.end(),
                                          [](const auto& idcounter1, const auto& idcounter2) {
                                            return idcounter1.second < idcounter2.second;
                                          });

    chipidbimap.insert(refClusterID, get<int>(maxIDCounter->first));
  }

  chipIDbimap finalchipidbimap;

  // reduces it to a binary, which it already is and since it's a hacky solution i am okay with it
  if (chipidbimap.getchiptocid().find(1) == chipidbimap.getchiptocid().end()) {
    const ClusterID& cid = find_if(refLbdCounter.begin(), refLbdCounter.end(),
                                    [&chipidbimap](const auto& refLbdCounterPair) {
                                      return refLbdCounterPair.first != chipidbimap.getchiptocid().find(-1)->second;
                                    })->first;
    finalchipidbimap.insert(cid, 1);
    finalchipidbimap.insert(chipidbimap.getchiptocid().find(-1)->second, -1);
  } else if (chipidbimap.getchiptocid().find(-1) == chipidbimap.getchiptocid().end()) {
    const ClusterID& cid = find_if(refLbdCounter.begin(), refLbdCounter.end(),
                                    [&chipidbimap](const auto& refLbdCounterPair) {
                                      return refLbdCounterPair.first != chipidbimap.getchiptocid().find(1)->second;
                                    })->first;
    finalchipidbimap.insert(cid, -1);
    finalchipidbimap.insert(chipidbimap.getchiptocid().find(1)->second, 1);
  } else {
    finalchipidbimap = chipidbimap;
  }

  if (finalchipidbimap.getcidtochip().size() != finalchipidbimap.getchiptocid().size()) {
    string msg = "Error: chipidbimap is not bijective:\n";
    msg += "\tcidtochip: ";
    for (const auto& [id, chip] : finalchipidbimap.getcidtochip()) {
      if (holds_alternative<int>(id)) {
        msg += to_string(get<int>(id));
      } else {
        msg += get<const string>(id);
      }
      msg += " -> " + to_string(chip) + ", ";
    }
    msg += "\n\tchiptocid: ";
    for (const auto& [chip, id] : finalchipidbimap.getchiptocid()) {
      msg += to_string(chip) + " -> ";
      if (holds_alternative<int>(id)) {
        msg += to_string(get<int>(id));
      } else {
        msg += get<const string>(id);
      }
      msg += ", ";
    }
    throw runtime_error(msg);
  }
  
  return finalchipidbimap;

#else

  const VerticesToLabel refVertices = vtlfromVertices(getaVertexforeachLabel(vertices));
  const LabeledVertices lbdVertices = auxrchip(refVertices, experts);

  chipIDbimap chipidmap;

  for (const VertexToLabel& vtl : refVertices) {
    for (const LabeledVertex& lbdv : lbdVertices) {
      if (lbdv.id == vtl.id) {
        chipidmap.insert(vtl.expectedclusterid, get<int>(lbdv.clusterid));
        break;
      }
    }
  }

  return chipidmap;

#endif
}

template <typename VT>
const VT getuptoNVerticesforeachLabel(const VT& vertices, const size_t n)
{
  VT result;

  map<ClusterID, size_t> lcount;

  for (const auto& v : vertices) {

    lcount[v.cluster->id] ++;

    if (lcount[v.cluster->id] <= n) {
      result.push_back(v);
    }

  }

  return result;
}

const VerticesToLabel vtlfromVertices(const Vertices& vertices)
{
  VerticesToLabel result;
  result.reserve(vertices.size());

  transform(vertices.begin(), vertices.end(),
            back_inserter(result),
            [](const Vertex& v) {
              return VertexToLabel(v.id, v.coordinates, v.cluster->id);
            });

  return result;
}

const LabeledVertices auxrchip(const VerticesToLabel& vertices, const Experts& experts)
{
  LabeledVertices labeledVertices;

  labeledVertices.reserve(vertices.size());

  for (const auto& vertex : vertices) {

    const Expert& closestExpert = *min_element(experts.begin(), experts.end(),
                                                [&vertex](const Expert& expert1, const Expert& expert2) {
                                                  return squaredDistance(vertex.coordinates, expert1.midpoint)
                                                          < squaredDistance(vertex.coordinates, expert2.midpoint);
                                                });

    const double separation = inner_product(vertex.coordinates.begin(), vertex.coordinates.end(),
                                            closestExpert.differences.begin(), -closestExpert.bias);

    const ClusterID clusterid = (separation > 0) - (separation < 0);

    labeledVertices.emplace_back(vertex.id, vertex.coordinates, clusterid);
  }

  return labeledVertices;
}
