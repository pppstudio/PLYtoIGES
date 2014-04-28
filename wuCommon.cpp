#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "wuCommon.h"

// ===========================================================================
//  wuSplit
// ===========================================================================
std::vector<std::string> wuSplit(const std::string &str, char delim){
  std::vector<std::string> res;
  size_t current = 0, found;
  while((found = str.find_first_of(delim, current)) != std::string::npos){
    res.push_back(std::string(str, current, found - current));
    current = found + 1;
  }
  res.push_back(std::string(str, current, str.size() - current));
  return res;
}