#include "stringHelpers.hpp"

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

const string trim(const string& s)
{
  const auto begin = find_if_not(s.begin(), s.end(), [](const unsigned char ch) {
    return isspace(ch);
  });
  
  const auto end = find_if_not(s.rbegin(), s.rend(), [](const unsigned char ch) {
    return isspace(ch);
  }).base();

  if (begin >= end) {
    return "";
  }
  
  return string(begin, end);
}


vector<string> split(const string& s, const char delimiter)
{
  vector<string> tokens;
  string token;
  istringstream tokenStream(s);

  while(getline(tokenStream, token, delimiter)) {
    tokens.push_back(token);
  }

  return tokens;
}

const string outputPathFromInputPath(const string& input_path) {
  const string input_file_without_path = input_path.substr(input_path.find_last_of("/\\") + 1);
  return "./output/" + input_file_without_path;
}
