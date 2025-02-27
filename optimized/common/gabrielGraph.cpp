#include "gabrielGraph.hpp"

#include <limits>
#include <algorithm>
#include <stdexcept>

#include "squaredDistance.hpp"

using namespace std;

void computeGabrielGraph(Vertices &vertices)
{
  const size_t vertexqtty = vertices.size();

  for (size_t i = 0; i < vertexqtty; ++ i) {
    for (size_t j = i + 1; j < vertexqtty; ++ j) {
      
      const float distancesq = squaredDistance(vertices[i].coordinates, vertices[j].coordinates);

      bool isGabrielEdge = true;

      for (size_t k = 0; k < vertexqtty; ++ k) {
        if (k == i || k == j) {
          continue;
        }

        const float distancesq1 = squaredDistance(vertices[i].coordinates, vertices[k].coordinates);
        const float distancesq2 = squaredDistance(vertices[j].coordinates, vertices[k].coordinates);

        if (distancesq > distancesq1 + distancesq2) {
          isGabrielEdge = false;
          break;
        }
      }

      if (isGabrielEdge) {
        const ClusterID viCid = vertices[i].cluster->id;
        const ClusterID vjCid = vertices[j].cluster->id;

        bool isSE = viCid != vjCid;

        vertices[i].adjacencyList.push_back({&vertices[j], isSE});
        vertices[j].adjacencyList.push_back({&vertices[i], isSE});
      }

    }
  }
}
