#ifndef STRINGHELPERS_HPP
#define STRINGHELPERS_HPP

#include <string>
#include <vector>

enum DIRECTION { FORWARD, BACKWARD };

const std::string trim(const std::string& s);
std::vector<std::string> split(const std::string& s, const char delimiter);
const std::string outputPathFromInputPath(const std::string& input_path);

#endif // STRINGHELPERS_HPP
