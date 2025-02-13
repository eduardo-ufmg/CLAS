#include "graphTypes.hpp"

bool Vertex::operator<(const Vertex& other) const
{
  return id < other.id;
}

bool Vertex::operator==(const Vertex& other) const
{
  return id == other.id;
}
