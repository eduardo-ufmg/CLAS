#include "strIDtochip.hpp"

using namespace std;

void strIDchipMap::insert(const string& strid, int chip) {
  stridtochip[strid] = chip;
  chiptostrid[chip] = strid;
}

int strIDchipMap::getchip(const string& strid) const {
  return stridtochip.at(strid);
}

const string& strIDchipMap::getstrid(int chip) const {
  return chiptostrid.at(chip);
}
