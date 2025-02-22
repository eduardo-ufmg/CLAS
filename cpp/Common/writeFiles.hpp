#ifndef WRITEFILES_H
#define WRITEFILES_H

#include <vector>
#include <map>
#include <string>

#include "graphTypes.hpp"

int writeGabrielGraphToFile(ClusterMap clusters, const std::string filename_with_path);
int writeVerticesToFile(ClusterMap clusters, const std::string filename_with_path);
int writeExpertsToFile(std::vector<Expert> experts, const std::string filename_with_path);
int writeClassifiedVertices(ClassifiedVertices vertices, const std::string filename_with_path);

#endif // WRITEFILES_H
