#ifndef LINUX_HELPER_H
#define LINUX_HELPER_H

#include <fstream>
#include <regex>
#include <string>

#include "linux_parser.h"
#include "linux_consts.h"

using std::string;
using std::vector;

namespace LinuxHelper {
template <typename T>
T GetValueByKey(string const &filter, string const &filename) {
  string line, key;
  T value;

  std::ifstream stream(LinuxConsts::kProcDirectory + filename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == filter) {
          return value;
        }
      }
    }
  }
  return value;
};
template <typename T>
T GetValue(string const &filename) {
  string line;
  T value;

  std::ifstream stream(LinuxConsts::kProcDirectory + filename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;
  }
  return value;
};
}  // namespace LinuxHelper
#endif