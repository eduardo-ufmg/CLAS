#include "stringHelpers.hpp"

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

string trim(const string &s)
{
  auto begin = find_if_not(s.begin(), s.end(), [](unsigned char ch) {
    return isspace(ch);
  });
  
  auto end = find_if_not(s.rbegin(), s.rend(), [](unsigned char ch) {
    return isspace(ch);
  }).base();

  if (begin >= end) {
    return "";
  }
  
  return string(begin, end);
}


vector<string> split(const string &s, const char delimiter)
{
  vector<string> tokens;
  string token;
  istringstream tokenStream(s);

  while(getline(tokenStream, token, delimiter)) {
    tokens.push_back(token);
  }

  return tokens;
}

vector<string> split(const string &s, const string &delimiter)
{
  vector<string> tokens;
  size_t start = 0;
  size_t end = 0;

  while((end = s.find(delimiter, start)) != string::npos) {
    tokens.push_back(s.substr(start, end - start));
    start = end + delimiter.length();
  }

  tokens.push_back(s.substr(start));
  return tokens;
}

vector<string> split(const string& line, const string& delimiter, const size_t& expectedPartSize)
{
  vector<string> parts = split(line, delimiter);

  if (parts.size() != expectedPartSize) {
    throw runtime_error("Expected " + to_string(expectedPartSize) + " parts, but got " + to_string(parts.size()));
  }

  return parts;
}

vector<string> split(const string &s, const char delimiter, const DIRECTION direction, const size_t max_splits)
{
  vector<string> tokens;
  string token;
  istringstream tokenStream(s);

  if (direction == FORWARD) {
    for (size_t i = 0; i < max_splits; i++) {
      if (!getline(tokenStream, token, delimiter)) {
        break;
      }
      tokens.push_back(token);
    }

    if (tokenStream) {
      getline(tokenStream, token);
      tokens.push_back(token);
    }
  } else {
    vector<string> reversed_tokens;
    while(getline(tokenStream, token, delimiter)) {
      reversed_tokens.push_back(token);
    }

    size_t tokens_size = reversed_tokens.size();
    for (size_t i = 0; i < max_splits; i++) {
      if (tokens_size - i - 1 < 0) {
        break;
      }
      tokens.push_back(reversed_tokens[tokens_size - i - 1]);
    }
  }

  return tokens;
}

string outputPathFromInputPath(const string &input_path) {
  string input_file_without_path = input_path.substr(input_path.find_last_of("/\\") + 1);
  return "./output/" + input_file_without_path;
}
