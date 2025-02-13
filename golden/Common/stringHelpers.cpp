#include "stringHelpers.hpp"

#include <string>
#include <vector>
#include <sstream>

using namespace std;

string trim(const string &s) {
  size_t start = s.find_first_not_of(" \t");

  if(start == string::npos) {
    return "";
  }

  size_t end = s.find_last_not_of(" \t");
  return s.substr(start, end - start + 1);
}

vector<string> split(const string &s, char delimiter) {
  vector<string> tokens;
  string token;
  istringstream tokenStream(s);
  while(getline(tokenStream, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}

string outputPathFromInputPath(const string &input_path) {
  string input_file_without_path = input_path.substr(input_path.find_last_of("/\\") + 1);
  return "./output/" + input_file_without_path;
}
