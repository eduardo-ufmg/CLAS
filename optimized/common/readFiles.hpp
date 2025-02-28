#ifndef READFILES_HPP
#define READFILES_HPP

#include <string>

#include "types.hpp"
#include "classifier.pb.h"

const std::string filenameFromPath(const std::string& path);
const std::string filenameNoExtension(const std::string& filename);
Vertices readDataset(const std::string& filename);

#endif // READFILES_HPP