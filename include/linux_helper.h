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
// GetValueByKey can be used parse lines with key value pairs
template <typename T>
T GetValueByKey(string const &targetKey, string const &kFilename) {
  string line, key;
  T value;
  std::ifstream stream(LinuxConsts::kProcDirectory + kFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == targetKey) {
          return value;
        }
      }
    }
  }
  return value;
};

// GetValue can be used parse lines without keys
template <typename T>
T GetValue(string const &kFilename) {
  string line;
  T value;
  std::ifstream stream(LinuxConsts::kProcDirectory + kFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;
  }
  return value;
};
}  // namespace LinuxHelper

#endif