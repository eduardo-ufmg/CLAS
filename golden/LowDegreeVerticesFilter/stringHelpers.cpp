#include "stringHelpers.hpp"

#include <string>

using namespace std;

string trim(const string &s) {
  size_t start = s.find_first_not_of(" \t");

  if(start == string::npos) {
    return "";
  }

  size_t end = s.find_last_not_of(" \t");
  return s.substr(start, end - start + 1);
}

string outputPathFromInputPath(const string &input_path) {
  string input_file_without_path = input_path.substr(input_path.find_last_of("/\\") + 1);
  string filtered_file_name = input_file_without_path.substr(0, input_file_without_path.find_last_of(".")) + "_filtered.csv";
  return "./output/" + filtered_file_name;
}
