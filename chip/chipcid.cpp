#include "chipcid.hpp"

#include <algorithm>
#include <numeric>

#include "squaredDistance.hpp"

using namespace std;

const Vertices getaVertexforeachLabel(const Vertices& vertices);
const VerticesToLabel vtlfromVertices(const Vertices& vertices);
const LabeledVertices auxrchip(const VerticesToLabel& vertices, const Experts& experts);

const chipIDbimap getchipIDmap(const Vertices& vertices, const Experts& experts)
{

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

}

const Vertices getaVertexforeachLabel(const Vertices& vertices)
{
  Vertices result;

  for (const Vertex& v : vertices) {

    for (const Vertex& w : result) {
      if (w.cluster->id == v.cluster->id) {
        goto next;
      }
    }

    result.push_back(v);
    next: continue;
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
