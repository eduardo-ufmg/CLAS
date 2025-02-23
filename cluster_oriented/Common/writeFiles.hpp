#ifndef WRITEFILES_H
#define WRITEFILES_H

#include <vector>
#include <map>
#include <string>

#include "graphTypes.hpp"

int writeGabrielGraphToFile(const ClusterMap& clusters, const std::string& filename_with_path);
int writeVerticesToFile(const ClusterMap& clusters, const std::string& filename_with_path);
int writeExpertsToFile(const Experts& experts, const std::string& filename_with_path);
int writeLabeledVertices(const LabeledVertices& vertices, const std::string& filename_with_path);

#endif // WRITEFILES_H
