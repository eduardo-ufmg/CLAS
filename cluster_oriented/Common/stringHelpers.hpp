#ifndef STRINGHELPERS_HPP
#define STRINGHELPERS_HPP

#include <string>
#include <vector>

enum DIRECTION { FORWARD, BACKWARD };

const std::string trim(const std::string& s);
std::vector<std::string> split(const std::string& s, const char delimiter);
std::vector<std::string> split(const std::string& s, const std::string& delimiter);
std::vector<std::string> split(const std::string& line, const std::string& delimiter, const size_t& expectedPartSize);
std::vector<std::string> split(const std::string& s, const char delimiter, const DIRECTION direction, const size_t max_splits = std::string::npos);
const std::string outputPathFromInputPath(const std::string& input_path);

#endif // STRINGHELPERS_HPP
