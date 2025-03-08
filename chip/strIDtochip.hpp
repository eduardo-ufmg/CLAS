#ifndef STRIDTOCHIP_HPP
#define STRIDTOCHIP_HPP

#include "types.hpp"

#include <map>
#include <string>

class strIDchipMap {
public:
  void insert(const std::string& strid, int chip);

  int getchip(const std::string& strid) const;

  const std::string& getstrid(int chip) const;

private:
  std::map<std::string, int> stridtochip;
  std::map<int, std::string> chiptostrid;
};

#endif // STRIDTOCHIP_HPP