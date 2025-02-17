#ifndef STRINGHELPERS_HPP
#define STRINGHELPERS_HPP

#include <string>
#include <vector>

std::string trim(const std::string &s);
std::vector<std::string> split(const std::string &s, char delimiter);
std::vector<std::string> split(const std::string &s, const std::string &delimiter);
std::string outputPathFromInputPath(const std::string &input_path);

#endif // STRINGHELPERS_HPP
