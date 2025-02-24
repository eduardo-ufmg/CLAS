#ifndef READFILES_HPP
#define READFILES_HPP

#include "types.hpp"

Vertices readTrainingData(const std::string &filename);
Vertices readSupportVertices(const std::string &filename);
Experts readExperts(const std::string &filename);
Vertices readUnlabeledVertices(const std::string &filename);

#endif // READFILES_HPP
