#ifndef READFILES_HPP
#define READFILES_HPP

#include "types.hpp"

Vertices readTrainingData(const std::string &filepath);
Vertices readSupportVertices(const std::string &filepath);
Experts readExperts(const std::string &filepath);
Vertices readUnlabeledVertices(const std::string &filepath);

#endif // READFILES_HPP
