#include "readFiles.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

#include "classifier.pb.h"

using namespace std;

Vertices readTrainingData(const string &filename)
{
  (void) filename;
  throw runtime_error("readTrainingData not implemented");
  return Vertices();
}

Vertices readSupportVertices(const string &filename)
{
  (void) filename;
  throw runtime_error("readSupportVertices not implemented");
  return Vertices();
}

Experts readExperts(const string &filename)
{
  (void) filename;
  throw runtime_error("readExperts not implemented");
  return Experts();
}

Vertices readUnlabeledVertices(const string &filename)
{
  (void) filename;
  throw runtime_error("readUnlabeledVertices not implemented");
  return Vertices();
}
