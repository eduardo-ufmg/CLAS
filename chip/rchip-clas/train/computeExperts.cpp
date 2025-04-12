#include "computeExperts.hpp"

#include <algorithm>

#include "isgabrielEdge.hpp"

using namespace std;

bool emplace_unique(Experts& experts, const ExpertID id, const Vertex& lowerVertex, const Vertex& higherVertex);

const Experts computeExperts(const Vertices& vertices)
{
  const size_t vertexqtty = vertices.size();

  Experts experts;
  ExpertID expertid = 0;

  for (size_t i = 0; i < vertexqtty; ++ i) {
    for (size_t j = i + 1; j < vertexqtty; ++ j) {
      
      const Vertex& vi = vertices[i];
      const Vertex& vj = vertices[j];

      if (vi.cluster == vj.cluster) {
        continue;
      }

      bool isGE = isGabrielEdge(vertices, vi, vj, vertexqtty);

      if (isGE) {
        const Vertex& lowerVertex = vi.id < vj.id ? vi : vj;
        const Vertex& higherVertex = vi.id > vj.id ? vi : vj;        
        emplace_unique(experts, expertid ++, lowerVertex, higherVertex);
      }

    }
  }

  return experts;
}

bool emplace_unique(Experts& experts, const ExpertID id, const Vertex& lowerVertex, const Vertex& higherVertex)
{
  auto it = find_if(experts.begin(), experts.end(), [&lowerVertex, &higherVertex](const std::unique_ptr<BaseExpert>& expert) {
    const auto* expertRCHIP = dynamic_cast<const ExpertRCHIP*>(expert.get());
    return expertRCHIP && expertRCHIP->edge.first->id == lowerVertex.id && expertRCHIP->edge.second->id == higherVertex.id;
  });

  if (it != experts.end()) {
    return false;
  }

  const Edge edge = make_pair(&lowerVertex, &higherVertex);
  experts.emplace_back(std::make_unique<ExpertRCHIP>(id, edge));

  return true;
}
