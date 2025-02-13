#ifndef WRITEFILES_H
#define WRITEFILES_H

#include <vector>
#include <map>
#include <string>

#include "graphTypes.hpp"

int writeGraphToFile(const ClusterMap& clusters, const std::string& file_name_with_path);

#endif // WRITEFILES_H
